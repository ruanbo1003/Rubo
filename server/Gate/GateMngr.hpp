/*
 * GameMngr.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_GATE_GATEMNGR_HPP_
#define SERVER_GATE_GATEMNGR_HPP_

#include <server/Gate/GateIO.hpp>
#include "comm/global.hpp"
#include "util/IThread.hpp"

class ServerBase;
using ServerBasePtr = tr1::shared_ptr<ServerBase>;

/*
 * GameMngr会运行一个线程，这个线程会管理IO事件
 */
class GateMngr : public IThread
{
private:
    ServerBasePtr _server;
    GateIOPtr _io_mngr;

public:
    GateMngr(const ServerBasePtr& server);
    virtual ~GateMngr();

private:
    void handler_network();
    void handler_netdata();

public:
    bool init();
    void unit();
    void thread_loop();
};
using GateMngrPtr = tr1::shared_ptr<GateMngr>;


#endif /* SERVER_GATE_GATEMNGR_HPP_ */
