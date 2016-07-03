/*
 * EpollOp.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_GATE_EPOLLOP_HPP_
#define SERVER_GATE_EPOLLOP_HPP_

#include "comm/global.hpp"
#include <sys/epoll.h>   //epoll

class EpollOp : public NonCopyable
{
#define EP_EVENT_SIZE  256

private:
    int _epoll_fd;

public:
    struct epoll_event _evs[EP_EVENT_SIZE];

public:
    EpollOp();
    virtual ~EpollOp();

public:
    bool epoll_add(int sfd);
    bool epoll_del(int sfd);
    bool epoll_in(int sfd);
    bool epoll_out(int sfd);

public:
    int wait(int time_out=100);
    bool init();
    void unit();

};
using EpollOpPtr = tr1::shared_ptr<EpollOp>;









#endif /* SERVER_GATE_EPOLLOP_HPP_ */

















