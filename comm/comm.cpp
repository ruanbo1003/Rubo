/*
 * comm.cpp
 *
 *      Author: ruanbo
 */

#include <comm/comm.hpp>

UserMsgPtr gen_msg(int msg_id)
{
    static MsgCenterPtr msg_center = MsgCenterPtr(new MsgCenter());

    return msg_center->gen_msg(msg_id);
}

//template<class T>
//std::tr1::shared_ptr<T> cast_msg(const MsgPtr& msg)
//{
//    return std::tr1::static_pointer_cast<T>(msg);
//}







