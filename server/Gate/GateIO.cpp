/*
 * IOMngr.cpp
 *
 *      Author: ruanbo
 */

#include <sys/ioctl.h>
#include <netinet/if_ether.h> // arpreq
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  // addrinfo
#include <fcntl.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <string.h>   //bzero()
#include <unistd.h>   // close()
#include <errno.h>
#include <sys/epoll.h>   //epoll
#include <limits.h>
#include <server/Gate/GateIO.hpp>

#include "server/ServerBase.hpp"
#include "comm/comm.hpp"

GateIO::GateIO(const ServerBasePtr& server)
{
    _server = server;

    _ip_addr = "";
    _ip_port = 0;
    _tcp_op = TcpOpPtr(new TcpOp(TCP_TYPE_SERVER));
}

GateIO::~GateIO()
{
    Log("~IOMngr");
}



int GateIO::on_accept()
{
    SocketNodePtr conn_node = _tcp_op->accept();
    if(!conn_node)
    {
        return -1;
    }

    UserPtr user = UserPtr(new User());
    user->_sock = conn_node;

    Log("GateIO new connection:%d", conn_node->_sfd);

    _unlogin_users.insert(make_pair(conn_node->_sfd, user));
    return conn_node->_sfd;
}

void GateIO::epoll_one()
{
    int ev_num = _tcp_op->epoll_wait();

    for(auto i=0; i<ev_num; ++i)
    {
        epoll_event & cur_ev = _tcp_op->_epoll->_evs[i];

        int sock_fd = cur_ev.data.fd;
        if(sock_fd == _tcp_op->_sock_fd)
        {
            on_accept();
        }
        else
        {
            UserPtr user = get_unlogin_user(sock_fd);
            if(user)
            {
                unlogin_sockop(user, cur_ev.events);
            }
            else
            {
                logined_sockop(sock_fd, cur_ev.events);
            }
        }
    }
}

void GateIO::handler_unlogin_msgs()
{
    list<UserMsgPtr> msgs;
    for(auto &user : _unlogin_reads)
    {
        SocketNodePtr sock = user->_sock;
        msgs = _server->_msg_center->parse_package(user);

        if(msgs.size() == 0)
        {
            continue;
        }
        if(msgs.size() > 1)
        {
            LogError("Gate 未登陆用户只能有一个未处理消息");
            kick_out(user);
        }
        else if(msgs.size()==1 && sock->_rd_buff->data_size() != 0)
        {
            LogError("Gate未登陆用户只有一个消息，但还有多于的数据");
            kick_out(user);
        }
        else
        {
            UserMsgPtr msg = msgs.front();
            if(msg->_msg_id != (int)Proto_System_VerifyReq && msg->_msg_id != (int)Proto_User_LoginReq
                    && msg->_msg_id != Proto_User_RegisterReq)
            {
                LogError("未认证/登陆的连接只能首先发送认证/登陆/注册消息");
                kick_out(user);
                continue;
            }

            MsgFuncPtr msg_func = _server->get_msg_handler(msg->_msg_id);
            if(!msg_func)
            {
                LogError("can not find func for msg id:%d", msg->_msg_id);
            }
            else
            {
                msg_func(msg, user, _rsp_msgs);
            }
            Log("IOMngr msg:%d", msg->_msg_id);
        }

        msgs.clear();
    }
}

void GateIO::handler_replys()
{
    for(auto &msg : _rsp_msgs)
    {
        int sfd = msg->_sfd;
        UserPtr user = get_unlogin_user(sfd);
        if(!user)
        {
            continue;
        }

        if(msg->_msg_id==(int)Proto_System_VerifyRsp)
        {
            auto rsp = cast_msg<VerifyRsp>(msg->_msg);
            if(rsp && rsp->isok() == true)
            {
                on_logic_verifyed(user);
            }
        }
        else if(msg->_msg_id==(int)Proto_User_LoginRsp)
        {
            auto rsp = cast_msg<LoginRsp>(msg->_msg);
            if(rsp && rsp->isok() == true)
            {
                on_user_logined(user, rsp->userid());
            }
        }

        _tcp_op->write(user->_sock, msg);
    }

    _rsp_msgs.clear();
}

void GateIO::notify_workers()
{
    for(auto &w : _wokers)
    {
        if(w->_events_io.empty() == true)
        {
            continue;
        }

        {
            std::unique_lock<std::mutex> lock(w->_cond->_mutex);
            w->move_io_event_to_in();
            lock.unlock();

            w->_cond->notify_all();
        }
    }
}

void GateIO::handler_data()
{
    for(auto &user : _unlogin_events)
    {
        SocketNodePtr sock = user->_sock;
        FD_EVENT ret = _tcp_op->event_op(sock);
        if(ret & FD_EVENT_ERROR)
        {
            on_conn_offline(user);
            continue;
        }

        if(ret & FD_EVENT_READ)
        {
            _unlogin_reads.push_back(user);
        }
    }

    _unlogin_events.clear();

    handler_unlogin_msgs();
}

void GateIO::unlogin_sockop(const UserPtr& user, uint32_t fdevent)
{
    if(fdevent & EPOLLIN || fdevent & EPOLLPRI)
    {
        user->_sock->_event |= FD_EVENT_READ;
    }
    if(fdevent & EPOLLOUT || fdevent & EPOLLERR || fdevent & EPOLLHUP)
    {
        user->_sock->_event |= FD_EVENT_WRITE;
    }

    _unlogin_events.push_back(user);
}

void GateIO::logined_sockop(int sock_fd, uint32_t fdevent)
{
    EventNodePtr one_event = EventNodePtr(new EventNode());
    one_event->_sfd = sock_fd;

    if(fdevent & EPOLLIN || fdevent & EPOLLPRI)
    {
        one_event->_event |= FD_EVENT_READ;
    }
    if(fdevent & EPOLLOUT || fdevent & EPOLLERR || fdevent & EPOLLHUP)
    {
        one_event->_event |= FD_EVENT_WRITE;
    }

    GateWorkerPtr work = get_worker(sock_fd);
    if(work)
    {
        work->_events_io.push_back(one_event);
    }
}

UserPtr GateIO::get_unlogin_user(int sfd)
{
    auto it = _unlogin_users.find(sfd);
    if(it != _unlogin_users.end())
    {
        return it->second;
    }

    return UserPtr();
}

GateWorkerPtr GateIO::get_worker(int sfd)
{
    for(auto &w : _wokers)
    {
        if(w->is_fd_in(sfd) >=1 || w->get_logicfd()==sfd)
        {
            return w;
        }
    }

    size_t min_num = UINT_MAX;
    GateWorkerPtr ret = GateWorkerPtr();
    for(auto &w : _wokers)
    {
        size_t work_fd_num = w->fd_num();
        if(work_fd_num < min_num)
        {
            ret = w;
            min_num = work_fd_num;
        }
    }

    return ret;
}

void GateIO::on_conn_offline(const UserPtr& user)
{
    // 未登陆的连接
    int sock_fd = user->_sock->_sfd;
    if(_unlogin_users.find(sock_fd) != _unlogin_users.end())
    {
        _unlogin_users.erase(sock_fd);
        return;
    }
}


void GateIO::kick_out(const UserPtr& user)
{
    _tcp_op->close_op(user->_sock->_sfd);
    on_conn_offline(user);
}


void GateIO::on_logic_verifyed(const UserPtr& user)
{
    int logic_fd = user->_sock->_sfd;

    auto uit = _unlogin_users.find(logic_fd);
    if(uit != _unlogin_users.end())
    {
        _unlogin_users.erase(uit);
    }

    _tcp_op->del_epev(logic_fd);

    GateWorkerPtr worker = GateWorkerPtr(new GateWorker(logic_fd));
    int widx = _wokers.size() + 1;
    string wname = "GateWorker " + std::to_string(widx);
    worker->set_tname(wname);
    if(worker->init() == false)
    {
        LogError("IOMngr init GateWorker failed");
        return;
    }

    _wokers.push_back(worker);

    worker->start();

    Log("Start thread[%s] for verifyed logic connection:%d", wname.c_str(), logic_fd);
}

void GateIO::on_user_logined(const UserPtr& user, int user_id)
{
    int fd = user->_sock->_sfd;
    auto uit = _unlogin_users.find(fd);
    if(uit != _unlogin_users.end())
    {
        _unlogin_users.erase(uit);
    }

    GateWorkerPtr worker = get_worker(fd);
    if(!worker)
    {
        LogError("Gate找不到 GateWorker for User connection");
        return;
    }

    EventNodePtr one_event = EventNodePtr(new EventNode());
    one_event->_uid = user->_uid;
    one_event->_sfd = fd;
    one_event->_event = FD_EVENT_CONNECT;

    worker->_events_io.push_back(one_event);
    worker->_fd_set.insert(fd);

    _tcp_op->del_epev(fd);

    Log("add a user[%d] to logic thread[%s]", user->_uid, worker->get_name().c_str());
}

void GateIO::set_ipport(const string& ip, int port)
{
    _ip_addr = ip;
    _ip_port = port;
}

bool GateIO::init()
{
    _tcp_op->set_addr(_ip_addr, _ip_port);
    if(_tcp_op->init() == false)
    {
        LogError("GateIO init TcpOp failed");
        unit();
        return false;
    }

    return true;
}

void GateIO::unit()
{
    if(_tcp_op)
    {
        _tcp_op->unit();
    }

    if(_server)
    {
        _server.reset();
    }
}

void GateIO::run()
{
    epoll_one();

    handler_data();

    handler_replys();

    notify_workers();
}

void GateIO::stop()
{
    for(auto &w : _wokers)
    {
        w->stop();
    }

    while(true)
    {
        bool need_wait = false;

        for(auto &w : _wokers)
        {
            if(w->is_over() == false)
            {
                w->_cond->notify_all();
                need_wait = true;
            }
        }

        if(need_wait == false)
        {
            break;
        }
        usleep(10000);
    }

    for(auto &w : _wokers)
    {
        w->join();
    }

}
