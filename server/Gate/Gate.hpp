/*
 * Gate2.hpp
 * Author: ruanbo
 */

#ifndef SERVER_GATE_GATE_HPP_
#define SERVER_GATE_GATE_HPP_

#include "comm/global.hpp"
#include "GateMngr.hpp"
#include "server/ServerBase.hpp"

class Gate : public ServerBase
{
#define GATE_BIND(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

public:
    GateMngrPtr _gatemngr;
public:
    Gate();
    virtual ~Gate();


public:
    bool init();
    void unit();
    bool run();
    void stop();

public:
    void register_handlers();

public:
    void on_user_login(const UserMsgPtr& pmsg, const UserPtr& user, std::list<UserMsgPtr>& rep_msgs);
    void on_user_register(const UserMsgPtr& pmsg, const UserPtr& user, std::list<UserMsgPtr>& rep_msgs);

    // 内部连接的验证
    void on_system_verify(const UserMsgPtr& pmsg, const UserPtr& user, std::list<UserMsgPtr>& rep_msgs);
    bool check_if_internal(const string& ip, const string& mac);
};

using GatePtr = tr1::shared_ptr<Gate>;










#endif /* SERVER_GATE_GATE_HPP_ */
