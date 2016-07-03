/*
 * LogicIO.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_LOGIC_LOGICIO_HPP_
#define SERVER_LOGIC_LOGICIO_HPP_

#include "comm/global.hpp"
#include "util/SocketNode.hpp"
#include "util/TcpOp.hpp"
#include "util/IThread.hpp"
#include "msgs/MsgCenter.hpp"

#include "LogicWorker.hpp"

class ServerBase;
using ServerBasePtr = tr1::shared_ptr<ServerBase>;
class Logic;
using LogicPtr = tr1::shared_ptr<Logic>;

class LogicIO : public IThread
{
private:
    LogicPtr _server;

    bool _verifyed;   //gate是否验证通过
    UserPtr _conn_user;

    struct epoll_event _evs[EP_EVENT_SIZE];
    TcpOpPtr _tcp_op;

    list<LogicWorkerPtr> _workers;

public:
    LogicIO(const ServerBasePtr& server);
    virtual ~LogicIO();

private:
    bool init_net();

    bool verify_to_gate();
    void on_verify_ret(const UserMsgPtr& user_msg);
public:
    bool init();
    void unit();
    void thread_loop();

    void add_worker(const LogicWorkerPtr& work);
    LogicWorkerPtr get_worker(int uid);
private:
    void handler_epevent();

    void handler_worker_msgs();
};

using LogicIOPtr = tr1::shared_ptr<LogicIO>;





#endif /* SERVER_LOGIC_LOGICIO_HPP_ */
