/*
 * Logic2.cpp
 *
 *      Author: ruanbo
 */

#include <server/Logic/Logic.hpp>
#include "comm/comm.hpp"
#include "util/MutexList.hpp"


Logic::Logic()
{
    _server_type = SERVER_GATE;
    _out_msgs = (MutexListPtr<UserMsgPtr>)(new MutexList<UserMsgPtr>());
}

Logic::~Logic()
{

}

void Logic::register_handlers()
{
    MsgIdFuncs GateHandlers[] =
    {

    };

    size_t func_num = sizeof(GateHandlers) / sizeof(MsgIdFuncs);
    for(size_t i=0; i<func_num; ++i)
    {
        register_handler(GateHandlers[i]._msg_id, GateHandlers[i]._msg_func);
    }
}

bool Logic::init()
{
    if(ServerBase::init() == false)
    {
        LogError("Logic2 init ServerBase failed");
        unit();
        return false;
    }

    _logic_io = LogicIOPtr(new LogicIO(shared_from_this()));
    if(_logic_io->init() == false)
    {
        LogError("Logic2 init LogicIO failed");
        unit();
        return false;
    }

    register_handlers();

    for(int i=1; i<=1; ++i)
    {
        LogicWorkerPtr work = LogicWorkerPtr(new LogicWorker(shared_from_this()));
        work->set_tname("LogicWorker " + std::to_string(i));

        if(work->init() == false)
        {
            LogError("%s init failed", work->get_cname());
            continue;
        }

        _workers.push_back(work);
        _logic_io->add_worker(work);
    }

    if(_workers.size() == 0)
    {
        LogError("Logic can not init even one LogicWorker.");
        unit();
        return false;
    }

    return true;
}


void Logic::unit()
{

}

bool Logic::run()
{
    if(_logic_io)
    {
        _logic_io->start();
    }

    for(auto &w : _workers)
    {
        w->start();
    }


    return true;
}

void Logic::stop()
{
    if(_logic_io)
    {
        _logic_io->stop();

        while(_logic_io->is_over() == false)
        {
            usleep(10000);
        }

        _logic_io->join();
    }

    for(auto &w : _workers)
    {
        bool need_wait = false;
        w->stop();
        while(w->is_over() == false)
        {
            need_wait = true;
            w->just_notify();
        }

        if(need_wait == true)
        {
            usleep(1000);
        }
        else
        {
            break;
        }
    }

    for(auto &w : _workers)
    {
        w->join();
    }
}

void Logic::add_msgs_to_io(const list<UserMsgPtr>& msgs)
{
    _out_msgs->add_more(msgs);
//    std::lock_guard<std::mutex> lock(_out_mtx);
//    _work_to_io.insert(_work_to_io.end(), msgs.begin(), msgs.end());
}

void Logic::io_get_msgs(list<UserMsgPtr>& ret_msgs)
{
    ret_msgs = _out_msgs->get_more();
//    std::lock_guard<std::mutex> lock(_out_mtx);
//    ret_msgs.insert(ret_msgs.end(), _work_to_io.begin(), _work_to_io.end());
//    _work_to_io.clear();
}








