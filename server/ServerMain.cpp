/*
 * ServerMain.cpp
 *
 *      Author: ruanbo
 */

#include <server/server.hpp>
#include <server/ServerMain.hpp>
#include <sys/resource.h>
#include <signal.h>
#include "comm/Log.hpp"

ServerMain::ServerMain()
{
    _run_gate = false;
    _run_logic = false;
}

ServerMain::~ServerMain()
{

}

bool ServerMain::set_res_limit()
{
    {
        struct rlimit core_limit;
        core_limit.rlim_cur = RLIM_INFINITY;
        core_limit.rlim_max = RLIM_INFINITY;

        if(setrlimit(RLIMIT_CORE, &core_limit) != 0)
        {
            return false;
        }
    }


    {
        struct rlimit file_limit;
        file_limit.rlim_cur = RLIM_INFINITY;
        file_limit.rlim_max = RLIM_INFINITY;
        if(setrlimit(RLIMIT_FSIZE, &file_limit) != 0)
        {
            return false;
        }
    }

    return true;
}

bool ServerMain::basic_init()
{
    srand((unsigned)time(NULL));

    signal(SIGINT, ServerMain::signal_handder);
    signal(SIGHUP, ServerMain::signal_handder);
    signal(SIGQUIT, ServerMain::signal_handder);

    return true;
}

void ServerMain::signal_handder(int signal)
{
    Log("if you want to stop the server. please enter quit");
}

bool ServerMain::init_system()
{
    if(set_res_limit() == false)
    {
        LogError("set system resource limit failed");
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


















