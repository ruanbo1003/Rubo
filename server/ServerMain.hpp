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
    bool set_res_limit();
    bool basic_init();

    static void signal_handder(int signal);
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
