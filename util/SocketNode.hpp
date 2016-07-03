/*
 * SocketNode.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_SOCKETNODE_HPP_
#define UTIL_SOCKETNODE_HPP_

#include <comm/global.hpp>
#include <string>
#include "RingBuff.hpp"

#define FD_EVENT  int
#define FD_EVENT_NONE     0
#define FD_EVENT_READ 0x01
#define FD_EVENT_WRITE 0x02
#define FD_EVENT_CONNECT 0x04
#define FD_EVENT_ERROR 0x08


enum ClientType
{
    CLIENT_TYPE_NULL = 0,
    CLIENT_TYPE_SYSTEM = 1,
    CLIENT_TYPE_USER = 2,

    CLIENT_TYPE_END = 99999
};


class EventNode
{
public:
    int _sfd;
    int _uid;
    int _event;

public:
    EventNode() : _sfd(0), _uid(0), _event(FD_EVENT_NONE)
    {

    }
};
using EventNodePtr = std::tr1::shared_ptr<EventNode>;


class SocketNode
{
public:
    ClientType _type;
    int _sfd;
    bool _can_wt;    // 是否可写

    FD_EVENT _event;
    FD_EVENT _op_ret;

    std::string _ip;
    int _port;

    RingBuffPtr _rd_buff;    // the read buffer of the socket
    RingBuffPtr _wt_buff;    // the write buffer of the socket


public:
    SocketNode();
    virtual ~SocketNode();

public:
    void release();

public:
    void recv_data(const char* buf, int len);
};

using SocketNodePtr = std::tr1::shared_ptr<SocketNode>;


class InternalIpMac
{
public:
    string _ip;
    string _mac;

    InternalIpMac() : _ip(""), _mac("")
    {

    }

    InternalIpMac(const string& ip, const string& mac) : _ip(ip), _mac(mac)
    {

    }
};

using InternalIpMacPtr = std::tr1::shared_ptr<InternalIpMac>;





#endif /* UTIL_SOCKETNODE_HPP_ */










