/*
 * EpollOp.cpp
 *
 *      Author: ruanbo
 */

#include <util/EpollOp.hpp>

EpollOp::EpollOp()
{
    _epoll_fd = 0;
}

EpollOp::~EpollOp()
{


}

bool EpollOp::epoll_add(int sfd)
{
    struct epoll_event ee = {0, { 0 }};
    ee.data.fd = sfd;
    ee.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;  //EPOLLOUT:socket默认都是可写的，所以这里不加 EPOLLOUT

    if(epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, sfd, &ee) == -1)
    {
        LogError("add new connection to epoll error!");
        return false;
    }

    return true;
}

bool EpollOp::epoll_del(int sfd)
{
    struct epoll_event ee = { 0, { 0 } };
    ee.events = EPOLLIN | EPOLLOUT;
    ee.data.u64 = 0;
    ee.data.fd = sfd;

    int ret = epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, sfd, &ee);

    return (ret >= 0);
}

bool EpollOp::epoll_in(int sfd)
{
    struct epoll_event ee = {0, { 0 }};
    ee.data.fd = sfd;
    ee.events = EPOLLIN | EPOLLPRI | EPOLLHUP | EPOLLERR;

    if(epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, sfd, &ee) == -1)
    {
        LogError("EPOLL_CTL_MOD in failed!");
        return false;
    }

    return true;
}

bool EpollOp::epoll_out(int sfd)
{
    struct epoll_event ee = {0, { 0 }};
    ee.data.fd = sfd;
    ee.events = EPOLLIN | EPOLLPRI | EPOLLHUP | EPOLLERR | EPOLLOUT;

    if(epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, sfd, &ee) == -1)
    {
        LogError("EPOLL_CTL_MOD failed!");
        return false;
    }

    return true;
}

int EpollOp::wait(int time_out)
{
    int nfds = epoll_wait(_epoll_fd, _evs, EP_EVENT_SIZE, time_out);
    return nfds;
}



bool EpollOp::init()
{
    _epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if(_epoll_fd < 0)
    {
        LogError("epoll_create1() error!");
        unit();
        return false;
    }

    return true;
}


void EpollOp::unit()
{
    if(_epoll_fd > 0)
    {
        close(_epoll_fd);
    }
}


















