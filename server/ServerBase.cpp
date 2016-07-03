/*
 * ServerBase.cpp
 *
 *      Author: ruanbo
 */

#include <server/ServerBase.hpp>

ServerBase::ServerBase()
{
    _server_type = SERVER_NULL;
    _mysqldb = MysqlDbPtr(new MysqlDb());
    _msg_center = MsgCenterPtr(new MsgCenter());
}

ServerBase::~ServerBase()
{
    Log("~ServerBase()");
}

bool ServerBase::init()
{
    if(_mysqldb->init() == false)
    {
        LogError("Gate init mysqldb fail");
        unit();
        return false;
    }

    if(_msg_center->init() == false)
    {
        LogError("ServerBase init MsgCenter failed");
        unit();
        return false;
    }

    return true;
}

void ServerBase::unit()
{
    if(_mysqldb)
    {
        _mysqldb->unit();
    }
    if(_msg_center)
    {
        _mysqldb->unit();
    }
}

void ServerBase::register_handler(ProtocolDefine proto_id, MsgFuncPtr func)
{
    _handlers[proto_id] = func;
}

MsgFuncPtr ServerBase::get_msg_handler(int msg_id)
{
    auto it = _handlers.find((ProtocolDefine)msg_id);
    if(it != _handlers.end())
    {
        return it->second;
    }

    return NULL;
}
