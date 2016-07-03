/*
 * LogicMngr.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_LOGIC_LOGICMNGR_HPP_
#define SERVER_LOGIC_LOGICMNGR_HPP_


#include "comm/global.hpp"
#include "LogicIO.hpp"

class ServerBase;
using ServerBasePtr = tr1::shared_ptr<ServerBase>;


class LogicMngr
{
private:
    ServerBasePtr _server;
    LogicIOPtr _io_mngr;

public:
    LogicMngr(const ServerBasePtr& server);
    virtual ~LogicMngr();

private:
    void handler_network();
    void handler_netdata();

public:
    bool init();
    void unit();
    bool thread_func();
};

using LogicMngrPtr = tr1::shared_ptr<LogicMngr>;



#endif /* SERVER_LOGIC_LOGICMNGR_HPP_ */
