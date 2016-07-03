/*
 * ProtocolDef.hpp
 *      Author: ruanbo
 */

#ifndef MSGCENTER_HPP_
#define MSGCENTER_HPP_


#include <msgs/UserMsg.hpp>
#include "util/SocketNode.hpp"
#include "util/LineBuffer.hpp"

class MsgCenter
{
private:
    std::map<ProtocolDefine, std::string> _msg_id_names;

    std::string _proto_dir;

    LineBufferPtr _parse_buff;       // 解析过程中的临时数据
    std::string _parse_string;

public:
    MsgCenter();
    virtual ~MsgCenter();

private:
    void register_protocol();

    bool import_proto_file(const std::string& path, const std::string& file_name);
    bool init_google_protos(const std::string& dir_path);
public:
    bool init();
    bool uinit();

public:
    UserMsgPtr gen_msg(ProtocolDefine proto_id);
    UserMsgPtr gen_msg(int msg_id);
private:
    UserMsgPtr gen_msg(const std::string& msg_name);

public:
    list<UserMsgPtr> parse_package(const UserPtr& user);
};

using MsgCenterPtr = std::tr1::shared_ptr<MsgCenter>;













#endif /* MSGCENTER_HPP_ */
