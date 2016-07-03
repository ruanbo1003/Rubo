/*
 * comm.hpp
 *
 *      Author: ruanbo
 */

#ifndef COMM_COMM_HPP_
#define COMM_COMM_HPP_


#include "msgs/MsgCenter.hpp"


UserMsgPtr gen_msg(int msg_id);


template<class T>
std::tr1::shared_ptr<T> cast_msg(const MsgPtr& msg)
{
    return std::tr1::static_pointer_cast<T>(msg);
}






#endif /* COMM_COMM_HPP_ */
