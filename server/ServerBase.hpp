/*
 * ServerBase.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_SERVERBASE_HPP_
#define SERVER_SERVERBASE_HPP_

#include <comm/global.hpp>
#include "msgs/UserMsg.hpp"
#include "msgs/MsgCenter.hpp"
#include "DB/MysqlDb.hpp"

enum ServerType
{
    SERVER_NULL = 0,

    SERVER_GATE = 1,
    SERVER_LOGIC = 2,

    SERVER_END
};

class ServerBase: public NonCopyable, public tr1::enable_shared_from_this<ServerBase>
{
private:
    std::map<ProtocolDefine, MsgFuncPtr> _handlers;

protected:
    ServerType _server_type;
    MysqlDbPtr _mysqldb;
public:
    MsgCenterPtr _msg_center;


public:
    ServerBase();
    virtual ~ServerBase();

public:
    bool init();
    void unit();

public:
    void register_handler(ProtocolDefine proto_id, MsgFuncPtr func);
    MsgFuncPtr get_msg_handler(int msg_id);
public:

};

using ServerBasePtr = tr1::shared_ptr<ServerBase>;

#endif /* SERVER_SERVERBASE_HPP_ */
