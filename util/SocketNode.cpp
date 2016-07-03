/*
 * SocketNode.cpp
 *
 *      Author: ruanbo
 */

#include <util/SocketNode.hpp>



SocketNode::SocketNode()
{
    release();

    _rd_buff = RingBuffPtr(new RingBuff());
    _wt_buff = RingBuffPtr(new RingBuff());
}

SocketNode::~SocketNode()
{
    release();
}

void SocketNode::release()
{
    _type = CLIENT_TYPE_NULL;
    _sfd = 0;
    _can_wt = true;      //init the writable to true

    _event = FD_EVENT_NONE;
    _op_ret = FD_EVENT_NONE;
    _ip = "";
    _port = 0;
}
