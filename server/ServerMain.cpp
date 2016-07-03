/*
 * ServerMain.cpp
 *
 *      Author: ruanbo
 */

#include <server/server.hpp>
#include <server/ServerMain.hpp>
#include <sys/resource.h>
#include "comm/Log.hpp"

ServerMain::ServerMain()
{
    _run_gate = false;
    _run_logic = false;
}

ServerMain::~ServerMain()
{

}

bool ServerMain::allow_coredown()
{
    struct rlimit core_limit;
    core_limit.rlim_cur = RLIM_INFINITY;
    core_limit.rlim_max = RLIM_INFINITY;

    return setrlimit(RLIMIT_CORE, &core_limit) == 0;
}

bool ServerMain::basic_init()
{
    srand((unsigned)time(NULL));

    return true;
}

bool ServerMain::init_system()
{
    if(allow_coredown() == false)
    {
        LogError("allow_coredown error");
        return false;
    }

    basic_init();

    return true;
}


bool ServerMain::init(const std::string& param)
{
    if(param == "")
    {
        _run_gate = true;
        _run_logic = true;
    }
    else
    {
        if(param.find("--") != 0)
        {
            Log("the run command is invalid");
            return false;
        }

        _run_gate = (param.find("gate") != string::npos);
        _run_logic = (param.find("logic") != string::npos);
    }

    if(_run_gate==false && _run_logic==false)
    {
        LogError("the run command is invalid");
        return false;
    }

    if(init_system() == false)
    {
        LogError("init_system fail");
        return false;
    }


    if(_run_gate)
    {
        _gate = GatePtr(new Gate());
        if(_gate->init() == false)
        {
            LogError("init gate fail");
            return false;
        }
    }

    if(_run_logic)
    {
        _logic = LogicPtr(new Logic());
        if(_logic->init() == false)
        {
            LogError("init logic error");
            return false;
        }
    }

    return true;
}

void ServerMain::stop_gate()
{
    if(_gate)
    {
        _gate->stop();
        _gate.reset();
    }
}

void ServerMain::stop_logic()
{
    if(_logic)
    {
        _logic->stop();
        _logic.reset();
    }
}

void ServerMain::run()
{
    if(_gate)
    {
        _gate->run();
    }

    if(_logic)
    {
        _logic->run();
    }


}

void ServerMain::stop()
{
    if(_logic)
    {
        _logic->stop();
    }
    if(_gate)
    {
        _gate->stop();
    }


    Log("end of ServerMain");
}


















