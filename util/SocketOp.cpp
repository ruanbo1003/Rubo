/*
 * SocketOP.cpp
 *
 *      Author: ruanbo
 */

#include <util/SocketOp.hpp>

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
#include "comm/global.hpp"

SocketOp::SocketOp()
{
    _rd_buff = LineBufferPtr(new LineBuffer());
    _wt_buff = LineBufferPtr(new LineBuffer());
}

SocketOp::~SocketOp()
{

}

sockaddr_in SocketOp::get_peer_name(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    std::fill_n((char*)&addr, sizeof(addr), 0);

    getpeername(fd, (struct sockaddr *)&addr, &len);

    return addr;
}

int SocketOp::accept(int fd, struct sockaddr_in & addr)
{
    socklen_t addr_len = sizeof(addr);

    return ::accept4(fd, (sockaddr*)&addr, &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
}


FD_EVENT SocketOp::read(const SocketNodePtr& sock_node)
{
    int sock_fd = sock_node->_sfd;
    _rd_buff->clean_data();
    ssize_t read_ret = 0;
    size_t loop_read_size = 0;
    int op_ret = FD_EVENT_NONE;

    while(true)
    {
        int left_read_size = _rd_buff->buff_size() - loop_read_size;
        int read_size = (left_read_size >= READ_SIZE) ? READ_SIZE : left_read_size;

        read_ret = ::read(sock_fd, _rd_buff->data()+loop_read_size, read_size);
        if(read_ret < 0)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else if(errno == EINTR)
            {
                continue;
            }
            else
            {
                op_ret = FD_EVENT_ERROR;
                break;
            }
        }
        else if(read_ret == 0)
        {
            op_ret = FD_EVENT_ERROR;
            break;
        }
        else
        {
            loop_read_size += read_ret;
            if(loop_read_size == _rd_buff->buff_size())
            {
                _rd_buff->append_size();
            }
        }
    }

    if(loop_read_size > 0)
    {
        sock_node->_rd_buff->add_data(_rd_buff->data(), loop_read_size);
    }

    Log("SocketOp. read size:%ld", loop_read_size);

    sock_node->_op_ret |= op_ret;
    return op_ret;
}

FD_EVENT SocketOp::send(const SocketNodePtr& sock, const char* data, int len)
{
    static int total_send_bytes = 0;

    if(sock->_can_wt == false)
    {
        Log("1 add data to sock wt_buff. len:%d", len);
        sock->_wt_buff->add_data(data, len);
        return true;
    }

    int sock_fd = sock->_sfd;
    int wt_len = 0;
    int one_len = 0;

    while(wt_len < len)
    {
        if(len - wt_len > WRITE_DATA_MAX)
        {
            one_len = WRITE_DATA_MAX;
        }
        else
        {
            one_len = len - wt_len;
        }

        int w_ret = ::write(sock_fd, data+wt_len, one_len);
        if(w_ret > 0)
        {
            total_send_bytes += w_ret;
        }
        else if(w_ret == 0)
        {
            return FD_EVENT_ERROR;
        }
        else
        {
            if(errno == EINTR)
            {
                continue;
            }
            else if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                //epoll_mod_out(sock);
                sock->_wt_buff->add_data(data+wt_len, len-wt_len);
                return FD_EVENT_WRITE;
            }
            else
            {
                return FD_EVENT_ERROR;
            }
        }

        wt_len += one_len;
    }

    return FD_EVENT_NONE;
}

FD_EVENT SocketOp::write(const SocketNodePtr& sock_node)
{
    sock_node->_can_wt = true;
    FD_EVENT op_ret = FD_EVENT_NONE;

    if(sock_node->_wt_buff->data_size() <= 0)
    {
        sock_node->_op_ret |= op_ret;
        return FD_EVENT_NONE;
    }

    size_t data_len = sock_node->_wt_buff->data_size();
    _wt_buff->clean_data();
    _wt_buff->append_size(data_len);
    sock_node->_wt_buff->read_del_begin(_wt_buff->data(), data_len);

    op_ret = send(sock_node, _wt_buff->data(), data_len);
    sock_node->_op_ret |= op_ret;
    return op_ret;
}


FD_EVENT SocketOp::write(const SocketNodePtr& sock_node, const UserMsgPtr& msg)
{
    int msg_len = msg->_msg->ByteSize();
    int msg_id = msg->_msg_id;
    int user_id = msg->_uid;

    _wt_buff->clean_data();
    _wt_buff->append_size(msg_len + 4 + 4 + 4);

    //先写消息头
    stdcopy(_wt_buff->data(), (char*)&msg_len, 4);
    stdcopy(_wt_buff->data()+4, (char*)&msg_id, 4);
    stdcopy(_wt_buff->data()+8, (char*)&user_id, 4);
    msg->_msg->SerializeToArray(_wt_buff->data()+12, msg_len);

    return send(sock_node, _wt_buff->data(), msg_len+12);
}

SocketNodePtr SocketOp::accept(int server_fd)
{
    sockaddr_in addr;
    std::fill_n((char*)&addr, sizeof(addr), 0);

    int conn_fd = accept(server_fd, addr);
    if(conn_fd < 0)
    {
        LogDebug("sccept error");
        return SocketNodePtr();
    }

    SocketNodePtr one_node = SocketNodePtr(new SocketNode());
    one_node->_sfd = conn_fd;
    one_node->_ip = inet_ntoa(addr.sin_addr);
    one_node->_port = addr.sin_port;
    one_node->_type = CLIENT_TYPE_NULL;

    return one_node;
}

int SocketOp::create_listen(const string& ip, int port)
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock_fd < 0)
    {
        LogError("SocketOp socket() failed");
        return -1;
    }

    if(set_options(sock_fd) == false)
    {
        LogError("SocketOp set_option() failed");
        close_fd(sock_fd);
        return -1;
    }

    sockaddr_in sock_addr;
    {
        bzero(&sock_addr, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = ip.empty()==true? INADDR_ANY : inet_addr(ip.c_str());
        sock_addr.sin_port = htons(port);
    }

    if(bind(sock_fd, (sockaddr*)&sock_addr, sizeof(sock_addr)) < 0)
    {
        LogError("SocketOp bind() failed");
        close_fd(sock_fd);
        return -1;
    }

    if(listen(sock_fd, SOMAXCONN) < 0)
    {
        LogError("SokcetOp listen() failed");
        close_fd(sock_fd);
        return -1;
    }

    return sock_fd;
}


int SocketOp::connect_to(const string& ip, int port)
{
    int sock_fd = 0;
    struct addrinfo hints, *serverinfo;

    std::fill_n((char*)&hints, sizeof(hints), 0);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rv = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &serverinfo);
    if(rv != 0)
    {
        LogError("getaddrinfo(%s:%d) failed", ip.c_str(), port);
        return -1;
    }

    for(auto p=serverinfo; p!=NULL; p=p->ai_next)
    {
        sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(sock_fd < 0)
        {
            continue;
        }

        if(connect(sock_fd, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(sock_fd);
            sock_fd = -1;
            continue;
        }

        break;
    }

    freeaddrinfo(serverinfo);

    return sock_fd;
}

void SocketOp::close_fd(int sock_fd)
{
    close(sock_fd);
}


bool SocketOp::set_nodelay(int fd)
{
    int on = 1;
    int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(on));

    if(ret == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool SocketOp::set_options(int fd)
{
    int flags = ::fcntl(fd, F_GETFL, 0);
    if(flags == -1)
    {
        LogError("set nonblock : F_GETFL error");
        return false;
    }

    flags = flags | O_NONBLOCK;
    int ret = fcntl(fd, F_SETFL, flags);
    if(ret == -1)
    {
        LogError("set nonblock : F_SETFL error");
        return false;
    }

    {
        int send_buff = 2304;
        setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void*)&send_buff, sizeof(int));
    }

    {
        int on = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on));
    }

    return true;
}




























