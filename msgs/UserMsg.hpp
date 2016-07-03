/*
 * MsgDef.h
 *
 *      Author: ruanbo
 */

#ifndef USERMSG_HPP_
#define USERMSG_HPP_

#include <comm/global.hpp>
#include <functional>
#include "protocols/login.pb.h"
#include "util/User.hpp"

enum ProtocolDefine
{
    ProtoDef_Null = 0,

    Proto_User_LoginReq = 1,
    Proto_User_RegisterReq = 2,
    Proto_System_VerifyReq = 3,
    Proto_User_LoginRsp = 4,
    Proto_User_RegisterRsp = 5,
    Proto_System_VerifyRsp = 6,
    Proto_User_Online_Req = 7,
    Proto_User_Offline_Req = 8,
    Proto_User_Dosth_Req = 9,
    Proto_User_Kickout_Req = 10,
    Proto_User_Dosth_Rsp = 11,

    Proto_System_Default = 999999,

    ProtoDef_End
};

using MsgPtr = std::tr1::shared_ptr<google::protobuf::Message>;
#define NullMsgPtr MsgPtr()

using LoginReqPtr = std::tr1::shared_ptr<LoginReq>;
using RegisterReqPtr = std::tr1::shared_ptr<RegisterReq>;
using VerifyReqPtr = std::tr1::shared_ptr<VerifyReq>;
using LoginRspPtr = std::tr1::shared_ptr<LoginRsp>;
using VerifyRspPtr = std::tr1::shared_ptr<VerifyRsp>;
using RegisterRspPtr = std::tr1::shared_ptr<RegisterRsp>;
using UserOnlineReqPtr = std::tr1::shared_ptr<UserOnlineReq>;
using UserOfflineReqPtr = std::tr1::shared_ptr<UserOfflineReq>;
using KickUserOutReqPtr = std::tr1::shared_ptr<KickUserOutReq>;
using DoSthReqPtr = std::tr1::shared_ptr<DoSthReq>;
using DoSthRspPtr = std::tr1::shared_ptr<DoSthRsp>;

class UserMsg;
using UserMsgPtr = std::tr1::shared_ptr<UserMsg>;

class UserMsg
{
public:
    int _uid;       // user id
    int _sfd;       // socket fd
    int _msg_id;    // message id
    MsgPtr _msg;

public:
    UserMsg();
    UserMsg(const MsgPtr& msg);
    UserMsg(const MsgPtr& msg, const UserMsgPtr& umsg);
    virtual ~UserMsg();
};



using MsgFuncPtr = std::function<void (UserMsgPtr, UserPtr, std::list<UserMsgPtr>&)>;

struct MsgIdFuncs
{
    ProtocolDefine _msg_id;
    MsgFuncPtr _msg_func;
};



#endif /* USERMSG_HPP_ */
