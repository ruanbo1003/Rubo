/*
 * Gate2.cpp
 *
 *      Author: ruanbo
 */

#include <server/Gate/Gate.hpp>
#include "comm/comm.hpp"


Gate::Gate()
{
    _server_type = SERVER_GATE;
}

Gate::~Gate()
{
    Log("~Gate2()");
}

bool Gate::init()
{
    if(ServerBase::init() == false)
    {
        LogError("Gate2 init ServerBase failed");
        unit();
        return false;
    }

    _gatemngr = GateMngrPtr(new GateMngr(shared_from_this()));
    if(_gatemngr->init() == false)
    {
        LogError("Gate2 init gatemanager failed");
        unit();
        return false;
    }

    register_handlers();

    return true;
}

void Gate::unit()
{
}



bool Gate::run()
{
    if(_gatemngr)
    {
        _gatemngr->start();
    }

    return true;
}

void Gate::stop()
{
    if(_gatemngr)
    {
        if(_gatemngr->is_over() == false)
        {
            Log("Gate2 stop GateMngr");
            _gatemngr->stop();

            while(_gatemngr->is_over() == false)
            {
                usleep(10000);
            }

            _gatemngr->join();
        }
        else
        {
            _gatemngr->unit();
        }

    }

    unit();
}

void Gate::register_handlers()
{
    MsgIdFuncs GateHandlers[] =
    {
            {Proto_User_LoginReq, GATE_BIND(Gate::on_user_login)},
            {Proto_User_RegisterReq, GATE_BIND(Gate::on_user_register)},
            {Proto_System_VerifyReq, GATE_BIND(Gate::on_system_verify)},
    };

    size_t func_num = sizeof(GateHandlers) / sizeof(MsgIdFuncs);
    for(size_t i=0; i<func_num; ++i)
    {
        register_handler(GateHandlers[i]._msg_id, GateHandlers[i]._msg_func);
    }
}


void Gate::on_user_login(const UserMsgPtr& pmsg, const UserPtr& user, std::list<UserMsgPtr>& rsp_msgs)
{
    auto msg = cast_msg<LoginReq>(pmsg->_msg);
    if(!msg)
    {
        return;
    }

    LoginRspPtr rsp = LoginRspPtr(new LoginRsp());

    int uid = _mysqldb->get_uid(msg->name(), msg->passwd());
    if(uid == -1)
    {
        rsp->set_isok(false);
        rsp->set_userid(-1);
    }
    else
    {
        rsp->set_isok(true);
        rsp->set_userid(uid);
    }

    user->_uid = uid;
    UserMsgPtr ret = UserMsgPtr(new UserMsg(rsp, pmsg));
    ret->_msg_id = 4;

    rsp_msgs.push_back(ret);

    Log("Gate2::on_user_login. user: [%d],%s, %s", uid, msg->name().c_str(), msg->passwd().c_str());
}



void Gate::on_user_register(const UserMsgPtr& pmsg, const UserPtr& user, std::list<UserMsgPtr>& rsp_msgs)
{
    auto msg = cast_msg<RegisterReq>(pmsg->_msg);
    if(!msg)
    {
        return;
    }

    RegisterRspPtr rsp = RegisterRspPtr(new RegisterRsp());
    bool is_register = _mysqldb->check_name_registered(msg->name());
    if(is_register == true)
    {
        rsp->set_isok(false);
        rsp->set_name("");
        rsp->set_accid(-1);
    }
    else
    {
        if(_mysqldb->register_user(msg->name(), msg->passwd(), 0) == false)
        {
            rsp->set_isok(false);
            rsp->set_name("");
            rsp->set_accid(-2);
        }
        else
        {
            int uid = _mysqldb->get_uid(msg->name(), msg->passwd());
            rsp->set_isok(true);
            rsp->set_name(msg->name());
            rsp->set_accid(uid);
        }
    }

    UserMsgPtr ret = UserMsgPtr(new UserMsg(rsp, pmsg));
    ret->_msg_id = 5;

    rsp_msgs.push_back(ret);

    Log("a user registered");
}

void Gate::on_system_verify(const UserMsgPtr& pmsg, const UserPtr& user, std::list<UserMsgPtr>& rsp_msgs)
{
    auto msg = cast_msg<VerifyReq>(pmsg->_msg);
    if(!msg)
    {
        return;
    }

    if(check_if_internal(user->_sock->_ip, msg->macaddr()) == false)
    {
        // T下线
        return;
    }

    VerifyRspPtr rsp = VerifyRspPtr(new VerifyRsp());
    rsp->set_isok(true);
    UserMsgPtr ret = UserMsgPtr(new UserMsg(rsp, pmsg));
    ret->_msg_id = 6;

    rsp_msgs.push_back(ret);

    Log("a system connection verified. ip:%s, mac:%s", user->_sock->_ip.c_str(), msg->macaddr().c_str());
}

bool Gate::check_if_internal(const string& ip, const string& mac)
{

    return true;
}










