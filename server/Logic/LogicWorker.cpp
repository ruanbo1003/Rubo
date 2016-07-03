/*
 * LogicWorker.cpp
 *
 *      Author: ruanbo
 */

#include <server/Logic/Logic.hpp>
#include <server/Logic/LogicWorker.hpp>
#include "server/ServerBase.hpp"
#include "comm/comm.hpp"

LogicWorker::LogicWorker(const ServerBasePtr& server)
{
    _cond = CondSyncPtr(new CondSync());
    _users = UserManagerPtr(new UserManager());
    _logic_server = dynamic_pointer_cast<Logic>(server);
}

LogicWorker::~LogicWorker()
{

}

bool LogicWorker::init()
{
    if(!_logic_server)
    {
        LogError("dynamic cast from ServerBase to Logic2 failed");
        unit();
        return false;
    }
    register_handlers();

    return true;
}

void LogicWorker::unit()
{
    if(_logic_server)
    {
        _logic_server.reset();
    }
    _handlers.clear();
    _msgs_in.clear();
    _io_msgs.clear();
    _out_msgs.clear();
}


void LogicWorker::thread_loop()
{
    handler_msgs();

    handler_reply();
}

bool LogicWorker::is_uid_in(int uid)
{
    std::lock_guard<std::mutex> lock(_u_mtx);

    return _uids.count(uid) >= 1;
}

size_t LogicWorker::user_num()
{
    std::lock_guard<std::mutex> lock(_u_mtx);
    return _uids.size();
}

void LogicWorker::add_user(int uid)
{
    std::lock_guard<std::mutex> lock(_u_mtx);
    _uids.insert(uid);
}

void LogicWorker::del_user(int uid)
{
    std::lock_guard<std::mutex> lock(_u_mtx);
    _uids.erase(uid);
}

void LogicWorker::register_handlers()
{
    LogicMsgIdFunc logic_handlers[] =
    {
            {Proto_User_Online_Req, LOGICWORKER_BIND(LogicWorker::on_user_online)},
            {Proto_User_Offline_Req, LOGICWORKER_BIND(LogicWorker::on_user_offline)},
            {Proto_User_Dosth_Req, LOGICWORKER_BIND(LogicWorker::on_user_dosth)},
    };

    size_t func_num = sizeof(logic_handlers) / sizeof(LogicMsgIdFunc);
    for(size_t i=0; i<func_num; ++i)
    {
        _handlers[logic_handlers[i]._msg_id] = logic_handlers[i]._msg_func;
    }
}

LogicHandlerPtr LogicWorker::get_msg_handler(int msg_id)
{
    auto it = _handlers.find((ProtocolDefine)msg_id);
    if(it != _handlers.end())
    {
        return it->second;
    }

    return NULL;
}

void LogicWorker::handler_one_msg(const UserMsgPtr& msg)
{
    Log("Thread[%s] handler msg[%d]", get_cname(), msg->_msg_id);

    // 这里，把handlers 该到LogicWorker里，而不是ServerBase，因为相关的数据都是在同一个LogicWorker里
    LogicHandlerPtr msg_func = get_msg_handler(msg->_msg_id);
    if(!msg_func)
    {
        LogError("[%s] can not find msg_function for msg[%d]", get_cname(), msg->_msg_id);
        return;
    }

    UserPtr user = _users->get_user_by_uid(msg->_uid);
    if(!user && msg->_msg_id != (int)Proto_User_Online_Req)
    {
        LogError("[%s] can not find user for msg[%d], uid[%d]", get_cname(), msg->_msg_id, msg->_uid);
        return;
    }

    msg_func(msg, user);
}

void LogicWorker::handler_msgs()
{
    list<UserMsgPtr> msgs;

    {
        std::unique_lock<std::mutex> lock(_cond->_mutex);
        _cond->_cond.wait(lock);

        msgs.swap(_msgs_in);

        lock.unlock();
    }

    for(auto &msg : msgs)
    {
        handler_one_msg(msg);
    }
}


void LogicWorker::handler_reply()
{
    _logic_server->add_msgs_to_io(_out_msgs);
    _out_msgs.clear();
}

void LogicWorker::notify_msg_in()
{
    std::unique_lock<std::mutex> lock(_cond->_mutex);
    _msgs_in.insert(_msgs_in.end(), _io_msgs.begin(), _io_msgs.end());
    _io_msgs.clear();
    lock.unlock();

    _cond->notify_all();
}

void LogicWorker::just_notify()
{
    _cond->notify_all();
}

void LogicWorker::on_user_online(const UserMsgPtr& pmsg, const UserPtr& user)
{
    auto msg = cast_msg<UserOnlineReq>(pmsg->_msg);
    if(!msg)
    {
        return;
    }

    UserPtr login_user = UserPtr(new User());
    login_user->_conn_status = CONNECT_LOGINED;
    login_user->_uid = msg->uid();
    _users->add_user(login_user);

    Log("[%s] handler user[%d] online", get_cname(), login_user->_uid);
}

void LogicWorker::on_user_offline(const UserMsgPtr& pmsg, const UserPtr& user)
{
    auto msg = cast_msg<UserOfflineReq>(pmsg->_msg);
    if(!msg)
    {
        return;
    }

    user->_uid = msg->uid();

    Log("[%s] handler user[%d] offline", get_cname(), user->_uid);
}

void LogicWorker::on_user_dosth(const UserMsgPtr& pmsg, const UserPtr& user)
{
    auto msg = cast_msg<DoSthReq>(pmsg->_msg);
    if(!msg)
    {
        return;
    }

    Log("[%s] handler user[%d] dosth[%s]", get_cname(), user->_uid, msg->sth().c_str());
    static int dosth_id = 100;
    auto rsp = _logic_server->_msg_center->gen_msg(Proto_User_Dosth_Rsp);
    rsp->_uid = user->_uid;
    auto rmsg = cast_msg<DoSthRsp>(rsp->_msg);
    rmsg->set_rspid(dosth_id++);
    rmsg->set_ret("DoSthRsp From " + get_name() + msg->sth());

    _out_msgs.push_back(rsp);
}















