/*
 * SocketOP.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_GATE_SOCKETOP_HPP_
#define SERVER_GATE_SOCKETOP_HPP_

#include "comm/global.hpp"
#include "util/SocketNode.hpp"
#include "util/LineBuffer.hpp"
#include "msgs/UserMsg.hpp"
#include <netinet/in.h>

/*
 *  socket 操作，就是 accept, read, write
 */
class SocketOp : public NonCopyable
{
#define READ_SIZE 64
#define READ_BUFF_SIZE 16
#define WRITE_BUFF_SIZE 256
#define WRITE_DATA_MAX  2048

private:
    LineBufferPtr _rd_buff;
    LineBufferPtr _wt_buff;

public:
    SocketOp();
    virtual ~SocketOp();

public:
    sockaddr_in get_peer_name(int fd);
    int accept(int fd, struct sockaddr_in & addr);

    FD_EVENT read(const SocketNodePtr& sock_node);
    FD_EVENT send(const SocketNodePtr& sock, const char* data, int len);
    FD_EVENT write(const SocketNodePtr& sock_node);
    FD_EVENT write(const SocketNodePtr& sock_node, const UserMsgPtr& msg);

    SocketNodePtr accept(int server_fd);

    int create_listen(const string& ip, int port);
    int connect_to(const string& ip, int port);

    void close_fd(int sock_fd);

public:
    bool set_nodelay(int fd);
    bool set_options(int fd);
};



using SocketOpPtr = tr1::shared_ptr<SocketOp>;





#endif /* SERVER_GATE_SOCKETOP_HPP_ */





