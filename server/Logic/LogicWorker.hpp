/*
 * LogicWorker.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_LOGIC_LOGICWORKER_HPP_
#define SERVER_LOGIC_LOGICWORKER_HPP_

#include "comm/global.hpp"
#include "util/IThread.hpp"
#include "msgs/UserMsg.hpp"
#include "util/CondSync.hpp"
#include "util/UserManager.hpp"
#include <mutex>

class ServerBase;
using ServerBasePtr = tr1::shared_ptr<ServerBase>;
class Logic;
using LogicPtr = tr1::shared_ptr<Logic>;

using LogicHandlerPtr = std::function<void (UserMsgPtr, UserPtr)>;
struct LogicMsgIdFunc
{
    ProtocolDefine _msg_id;
    LogicHandlerPtr _msg_func;
};


class LogicWorker : public IThread
{
#define LOGICWORKER_BIND(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2)

private:
//    ServerBasePtr _server;
    LogicPtr _logic_server;

    std::mutex _u_mtx;
    set<int> _uids;  // 此Worker处理的User消息, 应该用一个读写锁来进行互斥操作
    UserManagerPtr _users;

private:
    CondSyncPtr _cond;
    list<UserMsgPtr> _msgs_in;


public:
    list<UserMsgPtr> _io_msgs;  //这个只能由LogicIO来访问
    list<UserMsgPtr> _out_msgs;  // 此LogicWorker回复的消息

private:
    std::map<ProtocolDefine, LogicHandlerPtr> _handlers;


public:
    LogicWorker(const ServerBasePtr& server);
    virtual ~LogicWorker();


public:
    bool init();
    void unit();
    void thread_loop();


public:
    bool is_uid_in(int uid);
    size_t user_num();
    void add_user(int uid);
    void del_user(int uid);

private:
    void register_handlers();
    LogicHandlerPtr get_msg_handler(int msg_id);
public:
    void handler_one_msg(const UserMsgPtr& msg);
    void handler_msgs();
    void handler_reply();

    // 这个函数只能由IO调用 将IO msg加入到 msgs_in
    void notify_msg_in();
    void just_notify();

    void wait_cond();

public:
    void on_user_online(const UserMsgPtr& pmsg, const UserPtr& user);

    void on_user_offline(const UserMsgPtr& pmsg, const UserPtr& user);

    void on_user_dosth(const UserMsgPtr& pmsg, const UserPtr& user);
};

using LogicWorkerPtr = tr1::shared_ptr<LogicWorker>;



#endif /* SERVER_LOGIC_LOGICWORKER_HPP_ */











