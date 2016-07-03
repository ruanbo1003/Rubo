/*
 * ServerMain.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_SERVERMAIN_HPP_
#define SERVER_SERVERMAIN_HPP_

#include <comm/global.hpp>
#include <server/Gate/Gate.hpp>
#include <server/Logic/Logic.hpp>

class ServerMain
{
    bool _run_gate;
    bool _run_logic;
    LogicPtr _logic;
    GatePtr _gate;

public:
    ServerMain();
    virtual ~ServerMain();


private:
    bool allow_coredown();
    bool basic_init();

public:
    bool init_system();

    bool init(const std::string& param);


public:
    void run();
    void stop();

    void stop_gate();
    void stop_logic();

public:

};

using ServerMainPtr = std::tr1::shared_ptr<ServerMain>;


#endif /* SERVER_SERVERMAIN_HPP_ */
