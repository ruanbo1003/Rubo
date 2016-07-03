/*
 * GateWorker.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_GATE_GATEWORKER_HPP_
#define SERVER_GATE_GATEWORKER_HPP_

#include <comm/global.hpp>
#include "util/IThread.hpp"
#include "util/CondSync.hpp"
#include "util/SocketNode.hpp"
#include "util/UserManager.hpp"
#include "msgs/MsgCenter.hpp"
#include "util/TcpOp.hpp"


class GateWorker : public IThread
{
private:
    int _logic_fd;   // 一个GateWorker 对应一个 Logic 的连接
    EpollOpPtr _epoll_op;
    MsgCenterPtr _msg_center;
    TcpOpPtr _tcp_op;

    UserPtr _logic;         // Logic 连接
    UserManagerPtr _users;  // Users 连接

    list<UserMsgPtr> _user_msgs_in;   // User发送过来的消息，需要转发给Logic
    list<UserMsgPtr> _logic_msgs_in;  // Logic发送过来的消息，需要分发给User
public:
    CondSyncPtr _cond;
    list<EventNodePtr> _ioevs_in;   // IO线程给过来的读写事件

    std::mutex _fd_mtx; //用于对下面的 _fd_set 互斥操作
    set<int> _fd_set;   // 记录这个 gateworker 对应的user连接的socket fd。 注：这个只由 GateIO 来访问
    list<EventNodePtr> _events_io;  // 只由 GateIO 访问的事件，

    list<UserPtr> _epev_users;
    list<UserPtr> _rd_users;  // 处理过数据读取操作的user

public:
    GateWorker(int logicfd);
    virtual ~GateWorker();

public:
    void thread_loop();

    bool init();
    void unit();

public:
    bool is_fd_in(int fd);
    size_t fd_num();
    void add_fd(int fd);
    void del_fd(int fd);
private:
    void on_new_connect(const EventNodePtr& ev);

    void on_conn_offline(const UserPtr& user);
    void kick_out(const UserPtr& user);

    //处理IO线程同步过来的io事件，应该只有新增连接事件(connection)
    void handler_ioevent();
    void handler_iodata();

    void handler_epevent();

    void handler_epdata();

    // 在这里统一处理: 转发消息给Logic/分发消息给User/同步事件给IO
    void handler_msgs();

public:
    int get_logicfd()const;
    // 只由 IOMngr 在加锁的情况下调用
    void move_io_event_to_in();

};
using GateWorkerPtr = tr1::shared_ptr<GateWorker>;











#endif /* SERVER_GATE_GATEWORKER_HPP_ */
