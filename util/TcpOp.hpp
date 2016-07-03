/*
 * TcpOp.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_TCPOP_HPP_
#define UTIL_TCPOP_HPP_


#include "comm/global.hpp"
#include "SocketOp.hpp"
#include "EpollOp.hpp"
#include "SocketNode.hpp"

enum TCP_NET_TYPE
{
    TCP_TYPE_NULL = 0,

    TCP_TYPE_CLIENT = 1,
    TCP_TYPE_SERVER = 2,
    TCP_TYPE_EPOLL = 3,     //
};


class TcpOp : public NonCopyable
{
public:
    TCP_NET_TYPE _net_type;
    int _sock_fd;    // SERVER:监听 socket fd， CLIENT:连接的服务端的 socket fd

    string _ip_addr;
    int _ip_port;   // SERVER:监听的地址，端口， CLIENT:要连接的服务端地址，端口

    SocketOpPtr _sock;
    EpollOpPtr _epoll;

public:
    TcpOp(TCP_NET_TYPE type, const string& ip="", int port=0);
    virtual ~TcpOp();

private:
    bool init_server();
    bool init_client();

public:
    int get_sockfd()const;
    void set_addr(const string& ip, int port);

    bool init();
    void unit();

public:
    int epoll_wait();

    SocketNodePtr accept();
    void close_op(int sock_fd);
    FD_EVENT read(const SocketNodePtr& sock_node);
    FD_EVENT write(const SocketNodePtr& sock_node);
    FD_EVENT write(const SocketNodePtr& sock_node, const UserMsgPtr& msg);

    FD_EVENT event_op(const SocketNodePtr& node);

    void del_epev(int sock_fd);
    bool add_epev(int sock_fd);

    sockaddr_in get_peer_name(int fd);
};


using TcpOpPtr = tr1::shared_ptr<TcpOp>;


#endif /* UTIL_TCPOP_HPP_ */
