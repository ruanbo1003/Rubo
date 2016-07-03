/*
 * server_test.cpp
 *
 *      Author: ruanbo
 */


#include <comm/Log.hpp>
#include "server/ServerMain.hpp"
#include <iostream>


void server_tests(const std::string& param)
{
    ServerMainPtr smp(new ServerMain());

    if(smp->init(param) == false)
    {
        LogError("ServerMain init fail");
        return;
    }

    smp->run();

    std::string input = "";
    while(true)
    {
        std::cin >> input;
        if(input == "quit" || input == "QUIT")
        {
            break;
        }
        else if(input == "gateoff" || input == "GATEOFF")
        {
            smp->stop_gate();
        }
        else if(input == "logicoff" || input == "LOGICOFF")
        {
            smp->stop_logic();
        }
        else
        {

        }

        sleep(1);
    }

    smp->stop();
}















