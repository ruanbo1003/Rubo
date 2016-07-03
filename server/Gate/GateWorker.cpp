/*
 * GateWorker.cpp
 *
 *      Author: ruanbo
 */

#include <server/Gate/GateWorker.hpp>
#include "comm/comm.hpp"
#include <server/server.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex>


GateWorker::GateWorker(int logicfd)
{
    _logic_fd = logicfd;
    _cond = CondSyncPtr(new CondSync());
    _epoll_op = EpollOpPtr(new EpollOp());
    _tcp_op = TcpOpPtr(new TcpOp(TCP_TYPE_EPOLL));
    _msg_center = MsgCenterPtr(new MsgCenter());

    _users = UserManagerPtr(new UserManager());
}

GateWorker::~GateWorker()
{

}

void GateWorker::thread_loop()
{
    handler_ioevent();
//    handler_epevent();
    handler_epevent();

    handler_iodata();
    handler_epdata();

    handler_msgs();
}

bool GateWorker::init()
{
    if(_msg_center->init() == false)
    {
        LogError("%s init MsgCenter failed", get_cname());
        return false;
    }

    if(_tcp_op->init() == false)
    {
        LogError("[%s] init TcpOp failed", get_cname());
        unit();
        return false;
    }
    if(_tcp_op->add_epev(_logic_fd) == false)
    {
        LogError("[%s] TcpOp epoll_add logic fd failed", get_cname());
        unit();
        return false;
    }

    {
        SocketNodePtr one_node = SocketNodePtr(new SocketNode());
        one_node->_sfd = _logic_fd;
        one_node->_ip = "";
        one_node->_port = 0;
        one_node->_type = CLIENT_TYPE_SYSTEM;

        _logic = UserPtr(new User());
        _logic->_conn_status = CONNECT_LOGINED;
        _logic->_sock = one_node;

        _users->add_user(_logic);

        Log("New GateWorker Thread for logic fd:%d", _logic_fd);
    }

    return true;
}

void GateWorker::unit()
{
    if(_tcp_op)
    {
        _tcp_op->unit();
    }
}

bool GateWorker::is_fd_in(int fd)
{
    std::lock_guard<std::mutex> lock(_fd_mtx);

    return _fd_set.count(fd) >= 1;
}
size_t GateWorker::fd_num()
{
    std::lock_guard<std::mutex> lock(_fd_mtx);
    return _fd_set.size();
}
void GateWorker::add_fd(int fd)
{
    std::lock_guard<std::mutex> lock(_fd_mtx);
    _fd_set.insert(fd);
}

void GateWorker::del_fd(int fd)
{
    std::lock_guard<std::mutex> lock(_fd_mtx);
    _fd_set.erase(fd);
}

void GateWorker::on_new_connect(const EventNodePtr& ev)
{
    int fd = ev->_sfd;

    struct sockaddr_in addr = _tcp_op->get_peer_name(fd);

    SocketNodePtr one_node = SocketNodePtr(new SocketNode());
    one_node->_sfd = fd;
    one_node->_ip = inet_ntoa(addr.sin_addr);
    one_node->_port = addr.sin_port;
    one_node->_type = CLIENT_TYPE_USER;   // 默认都是USER，SYSTEM的连接会进行验证的

    UserPtr user(new User());
    user->_uid = ev->_uid;
    user->_conn_status = CONNECT_LOGINED;
    user->_sock = one_node;

    _users->add_conn(user);
    _tcp_op->add_epev(fd);

    {
        // 通知Logic有新的连接
        UserMsgPtr user_msg = _msg_center->gen_msg(Proto_User_Online_Req);
        auto msg = cast_msg<UserOnlineReq>(user_msg->_msg);
        msg->set_uid(ev->_uid);
        user_msg->_uid = ev->_uid;
        _user_msgs_in.push_back(user_msg);
    }

    Log("[%s] get new user,fd:%d, uid:%d", get_cname(), fd, ev->_uid);
}

void GateWorker::handler_ioevent()
{
    list<EventNodePtr> _ev_lists;
    {
        std::unique_lock<std::mutex> lock(_cond->_mutex);

        _ev_lists.swap(_ioevs_in);
        _ioevs_in.clear();

        lock.unlock();
    }

    for(auto &ev : _ev_lists)
    {
        if(ev->_event & FD_EVENT_CONNECT)
        {
            Log("Worker: FD_EVENT_CONNECT");
            on_new_connect(ev);
            continue;
        }
        else
        {
            LogError("IO同步给[%s]的事件不是FD_EVENT_CONNECT:%d", get_name().c_str(), ev->_event);
        }
    }
    _ev_lists.clear();
}

void GateWorker::handler_iodata()
{

}

void GateWorker::on_conn_offline(const UserPtr& user)
{
    SocketNodePtr sock = user->_sock;
    if(sock->_type == CLIENT_TYPE_SYSTEM)
    {
        LogError("%s logic connection offline!!!!!!!!", get_name().c_str());
        _users->del_conn(sock->_sfd);
    }
    else
    {
        Log("one user[%d], fd[%d] connection offline", user->_uid, sock->_sfd);
        _users->del_conn(sock->_sfd);

        UserMsgPtr user_msg = _msg_center->gen_msg(Proto_User_Offline_Req);
        user_msg->_uid = user->_uid;
        auto msg = cast_msg<UserOfflineReq>(user_msg->_msg);
        msg->set_uid(user->_uid);
        _user_msgs_in.push_back(user_msg);

        // 这里还要通知IO，从fd_set你删除这个User的fd就可以了，IO用这个来做决策
        del_fd(sock->_sfd);
    }
}

void GateWorker::kick_out(const UserPtr& user)
{
    _tcp_op->close_op(user->_sock->_sfd);
    on_conn_offline(user);
}

void GateWorker::handler_epevent()
{
    int ev_num = _tcp_op->epoll_wait();
    for(auto i=0; i<ev_num; ++i)
    {
        epoll_event & cur_ev = _tcp_op->_epoll->_evs[i];
        int sock_fd = cur_ev.data.fd;

        UserPtr user = _users->get_user_by_sock(sock_fd);
        if(!user)
        {
            LogError("[%s] can not find user for fd:%d", get_name().c_str(), sock_fd);
            continue;
        }

        SocketNodePtr sock_node = user->_sock;
        if(cur_ev.events & EPOLLIN || cur_ev.events & EPOLLPRI)
        {
            sock_node->_event |= FD_EVENT_READ;
        }
        if(cur_ev.events & EPOLLERR || cur_ev.events & EPOLLHUP || cur_ev.events & EPOLLOUT)
        {
            sock_node->_event |= FD_EVENT_WRITE;
        }

        if(sock_node->_event != FD_EVENT_NONE)
        {
            _epev_users.push_back(user);
        }
    }

    for(auto &user : _epev_users)
    {
        FD_EVENT ret = _tcp_op->event_op(user->_sock);

        if(ret & FD_EVENT_ERROR)
        {
            on_conn_offline(user);
            continue;
        }

        if(ret & FD_EVENT_READ)
        {
            _rd_users.push_back(user);
        }
    }

    _epev_users.clear();
}

void GateWorker::handler_epdata()
{
    list<UserMsgPtr> msgs;

    for(auto &user : _rd_users)
    {
        msgs = _msg_center->parse_package(user);
        for(auto &m : msgs)
        {
            LogDebug("[%s] recv one msg[%d]", get_cname(), m->_msg_id);

            // Logic发过来的数据
            if(user->_sock->_type == CLIENT_TYPE_SYSTEM)
            {
                _logic_msgs_in.push_back(m);
            }
            else  // User 发过来的数据
            {
                if(user->_uid != m->_uid)
                {
                    LogError("用户发送的消息头部UserId[%d]与登陆是的UserId[%d]不一致。", m->_uid, user->_uid);
                    continue;
                }

                _user_msgs_in.push_back(m);
            }
        }

        msgs.clear();
    }
}

void GateWorker::handler_msgs()
{
    // 转发user消息给 logic
    {
        for(auto &msg : _user_msgs_in)
        {
            Log("[%s] send msg[%d], uid[%d] to Logic.", get_cname(), msg->_msg_id, msg->_uid);

            _tcp_op->write(_logic->_sock, msg);
        }
        _user_msgs_in.clear();
    }

    // 将logic发送过来的消息分发User
    {
        for(auto &msg: _logic_msgs_in)
        {
            Log("Gate分发消息[%d]给User[%d]", msg->_msg_id, msg->_uid);
            UserPtr user = _users->get_user_by_uid(msg->_uid);
            if(user && user->_sock)
            {
                _tcp_op->write(user->_sock, msg);
            }
            else
            {
                LogError("GateWorker收到了Logic的发送给User[%d]的消息[%d]，但没有找到这个User", msg->_uid, msg->_msg_id);
            }
        }

        _logic_msgs_in.clear();
    }
}

int GateWorker::get_logicfd()const
{
    return _logic_fd;
}

void GateWorker::move_io_event_to_in()
{
    _ioevs_in.insert(_ioevs_in.end(), _events_io.begin(), _events_io.end());
    _events_io.clear();
}
















