/*
 * MsgDef.cpp
 *
 *      Author: ruanbo
 */

#include <msgs/UserMsg.hpp>

UserMsg::UserMsg()
{
    _uid = 0;
    _sfd = 0;
    _msg_id = 0;
}

UserMsg::UserMsg(const MsgPtr& msg)
{
    _uid = 0;
    _sfd = 0;
    _msg_id = 0;
    _msg = msg;
}

UserMsg::UserMsg(const MsgPtr& msg, const UserMsgPtr& umsg)
{
    _uid = umsg->_uid;
    _sfd = umsg->_sfd;
    _msg_id = umsg->_msg_id;
    _msg = msg;
}

UserMsg::~UserMsg()
{

}

