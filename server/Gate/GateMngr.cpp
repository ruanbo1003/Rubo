/*
 * GameMngr.cpp
 *
 *      Author: ruanbo
 */

#include <server/Gate/GateMngr.hpp>

GateMngr::GateMngr(const ServerBasePtr& server) : IThread("GateMngr")
{
    _server = server;
    _io_mngr = GateIOPtr(new GateIO(server));
}

GateMngr::~GateMngr()
{
    Log("~GateMngr");
}

void GateMngr::handler_network()
{
    _io_mngr->run();
}

void GateMngr::handler_netdata()
{

}

bool GateMngr::init()
{
    _io_mngr->set_ipport("127.0.0.1", 8881);
    if(_io_mngr->init() == false)
    {
        LogError("GateMngr init IOMngr failed");
        unit();
        return false;
    }

    return true;
}

void GateMngr::unit()
{
    Log("GateMngr::unit");

    _io_mngr->stop();

    _io_mngr->unit();

    if(_server)
    {
        _server.reset();
    }
}


void GateMngr::thread_loop()
{
    handler_network();

    handler_netdata();
}







