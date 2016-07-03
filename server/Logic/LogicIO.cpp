/*
 * LogicIO.cpp
 *
 *      Author: ruanbo
 */

#include <server/Logic/LogicIO.hpp>
#include "comm/comm.hpp"
#include "server/ServerBase.hpp"
#include <limits.h>
#include <server/Logic/Logic.hpp>

LogicIO::LogicIO(const ServerBasePtr& server)
{
    _server = dynamic_pointer_cast<Logic>(server);
    _verifyed = false;
    _tcp_op = TcpOpPtr(new TcpOp(TCP_TYPE_CLIENT));
}

LogicIO::~LogicIO()
{

}


bool LogicIO::init_net()
{
    _tcp_op->set_addr("127.0.0.1", 8881);
    if(_tcp_op->init() == false)
    {
        LogError("[%s] TcpOp init failed", get_cname());
        return false;
    }

    {
        _conn_user = UserPtr(new User());
        SocketNodePtr conn_node = SocketNodePtr(new SocketNode());
        conn_node->_type = CLIENT_TYPE_SYSTEM;
        conn_node->_sfd = _tcp_op->get_sockfd();
        _conn_user->_sock = conn_node;
    }

    if(verify_to_gate() == false)
    {
        LogError("LogicIO send verify msg to Gate failed");
        unit();
        return false;
    }

    return true;
}

bool LogicIO::verify_to_gate()
{
    UserMsgPtr req = _server->_msg_center->gen_msg(Proto_System_VerifyReq);
    auto msg = cast_msg<VerifyReq>(req->_msg);
    if(msg)
    {
        msg->set_macaddr("00:01:02:03:04:05:06:07");
    }
    else
    {
        return true;
    }

    FD_EVENT ret = _tcp_op->write(_conn_user->_sock, req);

    Log("verify to gate, ret:%d", ret);

    return ret == FD_EVENT_NONE;
}

void LogicIO::on_verify_ret(const UserMsgPtr& user_msg)
{
    auto msg = cast_msg<VerifyRsp>(user_msg->_msg);
    if(msg->isok() == true)
    {
        Log("gate verifyed [%s]", get_cname());
    }
    else
    {
        LogError("gate not verified [%s]", get_cname());
    }
}


bool LogicIO::init()
{
    set_tname("LogicIO");

    if(!_server)
    {
        LogError("LogicIO dynamic_cast ServerBase to Logic2 failed");
        uint();
        return false;
    }

    if(init_net() == false)
    {
        LogError("LogicIO init_net failed");
        unit();
        return false;
    }

    return true;
}

void LogicIO::unit()
{
    if(_tcp_op)
    {
        _tcp_op->unit();
    }

    _workers.clear();

    if(_server)
    {
        _server.reset();
    }
}


void LogicIO::thread_loop()
{
    handler_epevent();

    handler_worker_msgs();

    tmsleep(100);
}

void LogicIO::add_worker(const LogicWorkerPtr& work)
{
    _workers.push_back(work);
}

LogicWorkerPtr LogicIO::get_worker(int uid)
{
    for(auto &w : _workers)
    {
        if(w->is_uid_in(uid) == true)
        {
            return w;
        }
    }

    LogicWorkerPtr worker = LogicWorkerPtr();
    size_t min_num = UINT_MAX;
    for(auto &w : _workers)
    {
        size_t user_num = w->user_num();
        if(w->user_num() < min_num)
        {
            min_num = user_num;
            worker = w;
        }
    }

    return worker;
}


void LogicIO::handler_epevent()
{
    int ev_num = _tcp_op->epoll_wait();

    for(auto i=0; i<ev_num; ++i)
    {
        epoll_event & cur_ev = _tcp_op->_epoll->_evs[i];

        int sock_fd = cur_ev.data.fd;
        if(sock_fd != _tcp_op->get_sockfd())
        {
            LogError("[%s] 有不合法的可读事件. fd:%d", get_cname(), sock_fd);
            continue;
        }

        SocketNodePtr sock = _conn_user->_sock;
        if(cur_ev.events & EPOLLIN || cur_ev.events & EPOLLPRI)
        {
            sock->_event |= FD_EVENT_READ;
        }
        if(cur_ev.events & EPOLLOUT || cur_ev.events & EPOLLERR || cur_ev.events & EPOLLHUP)
        {
            sock->_event |= FD_EVENT_WRITE;
        }

        FD_EVENT ret = _tcp_op->event_op(sock);
        list<UserMsgPtr> msgs = _server->_msg_center->parse_package(_conn_user);
        for(auto &msg : msgs)
        {
            Log("LogicIO recv one msg[%d]", msg->_msg_id);

            if(msg->_msg_id == (int)Proto_System_VerifyRsp)
            {
                on_verify_ret(msg);
                continue;
            }

            LogicWorkerPtr work = get_worker(msg->_uid);
            work->_io_msgs.push_back(msg);
        }
    }

    for(auto &w : _workers)
    {
        if(w->_io_msgs.size() == 0)
        {
            continue;
        }

        w->notify_msg_in();
    }
}

void LogicIO::handler_worker_msgs()
{
    list<UserMsgPtr> msgs;
    _server->io_get_msgs(msgs);

    for(auto &msg : msgs)
    {
        _tcp_op->write(_conn_user->_sock, msg);

        Log("Logic rsp msg[%d] for user[%d]", msg->_msg_id, msg->_uid);
    }

    msgs.clear();
}
















