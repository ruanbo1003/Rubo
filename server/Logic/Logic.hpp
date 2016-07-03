/*
 * Logic2.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_LOGIC_LOGIC_HPP_
#define SERVER_LOGIC_LOGIC_HPP_


#include "comm/global.hpp"
#include "server/ServerBase.hpp"
#include "LogicIO.hpp"
#include "LogicWorker.hpp"
#include "util/MutexList.hpp"

class Logic : public ServerBase
{
#define LOGIC_BIND(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
private:
    LogicIOPtr _logic_io;
    list<LogicWorkerPtr> _workers;


    MutexListPtr<UserMsgPtr> _out_msgs;
    std::mutex _out_mtx;
    list<UserMsgPtr> _work_to_io;  // LogicWorker回复的消息，通过IO发送

public:
    Logic();
    virtual ~Logic();

private:
    void register_handlers();


public:
    bool init();
    void unit();
    bool run();
    void stop();

public:
    void add_msgs_to_io(const list<UserMsgPtr>& msgs);
    void io_get_msgs(list<UserMsgPtr>& ret_msgs);
};

using LogicPtr = tr1::shared_ptr<Logic>;



#endif /* SERVER_LOGIC_LOGIC_HPP_ */
