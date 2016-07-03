/*
 * TcpOp.cpp
 *
 *      Author: ruanbo
 */

#include <util/TcpOp.hpp>

TcpOp::TcpOp(TCP_NET_TYPE type, const string& ip, int port)
    : _net_type(type), _sock_fd(0), _ip_addr(ip), _ip_port(port)
{
    _sock = SocketOpPtr(new SocketOp());
    _epoll = EpollOpPtr(new EpollOp());
}

TcpOp::~TcpOp()
{


}

bool TcpOp::init_server()
{
    _sock_fd = _sock->create_listen(_ip_addr, _ip_port);

    Log("TcpOp::init_server. fd:%d", _sock_fd);

    if(_sock_fd < 0)
    {
        LogError("TcpOp[%d] server listen tcp[%s:%d] failed", _net_type, _ip_addr.c_str(), _ip_port);
        unit();
        return false;
    }

    return true;
}


bool TcpOp::init_client()
{
    for(int i=0; i<3; ++i)
    {
        _sock_fd = _sock->connect_to(_ip_addr, _ip_port);
        if(_sock_fd > 0)
        {
            break;
        }

        sleep(1);
    }

    Log("TcpClient connect to [%s:%d] ok", _ip_addr.c_str(), _ip_port);

    if(_sock->set_options(_sock_fd) == false)
    {
        LogError("TcpOp client set_options failed");
        return false;
    }

    if(_sock->set_nodelay(_sock_fd) == false)
    {
        LogError("TcpOp client set_nodelay failed");
        return false;
    }

    return true;
}

int TcpOp::get_sockfd()const
{
    return _sock_fd;
}

void TcpOp::set_addr(const string& ip, int port)
{
    _ip_addr = ip;
    _ip_port = port;
}

bool TcpOp::init()
{
    if(((_ip_addr=="" || _ip_port==0) && _net_type != TCP_TYPE_EPOLL) ||
            _net_type == TCP_TYPE_NULL)
    {
        LogError("TcpOp ip or port or tcp type is invalid");
        return false;
    }

    if(_epoll->init() == false)
    {
        LogError("TcpOp[%d] init EpollOp failed", _net_type);
        unit();
        return false;
    }

    if(_net_type == TCP_TYPE_CLIENT)
    {
        init_client();
    }
    else if(_net_type == TCP_TYPE_SERVER)
    {
        init_server();
    }
    else
    {
        //
    }

    if(_net_type != TCP_TYPE_EPOLL && _epoll->epoll_add(_sock_fd) == false)
    {
        LogError("TcpOp[%d] epoll add socket fd failed", _net_type);
        unit();
        return false;
    }

    return true;
}



void TcpOp::unit()
{
    if(_sock && _sock_fd > 0)
    {
        _sock->close_fd(_sock_fd);
    }

    if(_epoll)
    {
        _epoll->unit();
    }
}

int TcpOp::epoll_wait()
{
    return _epoll->wait();
}


SocketNodePtr TcpOp::accept()
{
    if(_net_type != TCP_TYPE_SERVER)
    {
        return SocketNodePtr();
    }

    SocketNodePtr conn_node = _sock->accept(_sock_fd);
    if(!conn_node)
    {
        return conn_node;
    }

    int conn_fd = conn_node->_sfd;
    if(_sock->set_options(conn_fd) == false)
    {
        Log("connect:set_options failed");
        _sock->close_fd(conn_fd);
        return SocketNodePtr();
    }
    if(_epoll->epoll_add(conn_fd) == false)
    {
        _sock->close_fd(conn_fd);
        return SocketNodePtr();
    }

    return conn_node;
}

void TcpOp::close_op(int sock_fd)
{
    _sock->close_fd(sock_fd);
    _epoll->epoll_del(sock_fd);
}

FD_EVENT TcpOp::read(const SocketNodePtr& sock_node)
{
    return _sock->read(sock_node);
}


FD_EVENT TcpOp::write(const SocketNodePtr& sock_node)
{
    return _sock->write(sock_node);
}

FD_EVENT TcpOp::write(const SocketNodePtr& sock_node, const UserMsgPtr& msg)
{
    return _sock->write(sock_node, msg);
}


FD_EVENT TcpOp::event_op(const SocketNodePtr& node)
{
    FD_EVENT ret = FD_EVENT_NONE;


    if(node->_event & FD_EVENT_READ)
    {
        ret |= read(node);
        ret |= FD_EVENT_READ;
    }

    if(node->_event & FD_EVENT_WRITE)
    {
        FD_EVENT wt_ret =  write(node);
        if((wt_ret & FD_EVENT_WRITE) == false)
        {
            _epoll->epoll_in(node->_sfd);
        }

        ret |= wt_ret;
    }
    else
    {
        if(node->_wt_buff->data_size() > 0)
        {
            FD_EVENT wt_ret =  write(node);
            if((wt_ret & FD_EVENT_WRITE) == true)
            {
                _epoll->epoll_out(node->_sfd);
            }

            ret |= wt_ret;
        }
    }


    node->_event = FD_EVENT_NONE;
    node->_op_ret = FD_EVENT_NONE;

    if(ret & FD_EVENT_ERROR)
    {
        close_op(node->_sfd);
        return FD_EVENT_ERROR;
    }

    return ret;
}

void TcpOp::del_epev(int sock_fd)
{
    _epoll->epoll_del(sock_fd);
}

bool TcpOp::add_epev(int sock_fd)
{
    return _epoll->epoll_add(sock_fd);
}


sockaddr_in TcpOp::get_peer_name(int fd)
{
    return _sock->get_peer_name(fd);
}







