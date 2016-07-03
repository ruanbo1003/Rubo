/*
 * IOMngr.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_GATE_GATEIO_HPP_
#define SERVER_GATE_GATEIO_HPP_

#include "comm/global.hpp"
#include "util/SocketNode.hpp"
#include "util/TcpOp.hpp"
#include "msgs/MsgCenter.hpp"
#include "GateWorker.hpp"

class ServerBase;
using ServerBasePtr = tr1::shared_ptr<ServerBase>;

class GateIO : public NonCopyable
{
#define EP_EVENT_SIZE  256
private:
    ServerBasePtr _server;
public:
    string _ip_addr;
    int _ip_port;
    TcpOpPtr _tcp_op;

    map<int, UserPtr> _unlogin_users;  // 未登陆(user)，没验证(system)的连接
    list<UserPtr> _unlogin_events;     // 未登陆连接的event
    list<UserPtr> _unlogin_reads;      // 读取过数据的Socket，下一步是解析

    list<UserMsgPtr> _rsp_msgs;

    list<GateWorkerPtr> _wokers;


public:
    GateIO(const ServerBasePtr& server);
    virtual ~GateIO();

private:
    int on_accept();

    void epoll_one();
    void handler_unlogin_msgs();
    void handler_replys();
    void notify_workers();
    void handler_data();
    // 未登陆连接的操作
    void unlogin_sockop(const UserPtr& user, uint32_t fdevent);
    // 已登陆连接的操作
    void logined_sockop(int sock_fd, uint32_t fdevent);

private:
    UserPtr get_unlogin_user(int sfd);
    GateWorkerPtr get_worker(int sfd);

    void on_conn_offline(const UserPtr& user);
    void kick_out(const UserPtr& user);
    void on_logic_verifyed(const UserPtr& user);
    void on_user_logined(const UserPtr& user, int user_id);
public:
    void set_ipport(const string& ip, int port);
    bool init();
    void unit();

    void run();

    void stop();
};

using GateIOPtr = tr1::shared_ptr<GateIO>;









#endif /* SERVER_GATE_GATEIO_HPP_ */
