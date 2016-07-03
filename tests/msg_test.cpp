/*
 * msg_test.cpp
 *
 *      Author: ruanbo
 */

#include <comm/global.hpp>
#include <msgs/MsgCenter.hpp>



void msg_tests()
{
    MsgCenterPtr proto = MsgCenterPtr(new MsgCenter());

    int msg_id = 1;
    UserMsgPtr msg = proto->gen_msg(msg_id);
    if(!msg)
    {
        LogError("get msg %d error", msg_id);
    }


}



