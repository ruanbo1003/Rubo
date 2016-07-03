/*
 * LogicMngr.cpp
 *
 *      Author: ruanbo
 */

#include <server/Logic/LogicMngr.hpp>

LogicMngr::LogicMngr(const ServerBasePtr& server)
{
    _server = server;
    _io_mngr = LogicIOPtr(new LogicIO(_server));
}

LogicMngr::~LogicMngr()
{


}

bool LogicMngr::init()
{
//    _io_mngr->set_ipport("127.0.0.1", 8881);
//    if(_io_mngr->init() == false)
//    {
//        LogError("GateMngr init IOMngr failed");
//        unit();
//        return false;
//    }

    return true;
}

void LogicMngr::unit()
{
    _server.reset();
}

bool LogicMngr::thread_func()
{


    return true;
}




















