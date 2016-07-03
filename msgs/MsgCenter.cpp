/*
 * ProtocolDef.cpp
 *
 *      Author: ruanbo
 */

#include <msgs/MsgCenter.hpp>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

#include <dirent.h>       //readdir(),
#include <sys/stat.h>     // S_ISDIR()

MsgCenter::MsgCenter()
{
    _proto_dir = "msgs/protocols";
    _parse_buff = LineBufferPtr(new LineBuffer());
    _parse_string = "";

    register_protocol();
}

MsgCenter::~MsgCenter()
{

}

void MsgCenter::register_protocol()
{
    _msg_id_names[Proto_User_LoginReq] = "LoginReq";
    _msg_id_names[Proto_User_RegisterReq] = "RegisterReq";
    _msg_id_names[Proto_System_VerifyReq] = "VerifyReq";
    _msg_id_names[Proto_User_LoginRsp] = "LoginRsp";
    _msg_id_names[Proto_User_RegisterRsp] = "RegisterRsp";
    _msg_id_names[Proto_System_VerifyRsp] = "VerifyRsp";
    _msg_id_names[Proto_User_Online_Req] = "UserOnlineReq";
    _msg_id_names[Proto_User_Offline_Req] = "UserOfflineReq";
    _msg_id_names[Proto_User_Kickout_Req] = "KickUserOutReq";
    _msg_id_names[Proto_User_Dosth_Req] = "DoSthReq";
    _msg_id_names[Proto_User_Dosth_Rsp] = "DoSthRsp";
}


bool MsgCenter::import_proto_file(const std::string& path, const std::string& file_name)
{
    google::protobuf::compiler::DiskSourceTree sourceTree;
    sourceTree.MapPath("", path);

    google::protobuf::compiler::Importer importer(&sourceTree, NULL);

    const google::protobuf::FileDescriptor* file = importer.Import(file_name);
    if(!file)
    {
        LogError("Import proto file:%s Error...", file_name.c_str());
        return false;
    }
    else
    {
        LogDebug("Import proto file:%s OK...", file_name.c_str());
    }

    return true;
}

bool MsgCenter::init_google_protos(const std::string& dir_path)
{
    DIR *dirp = opendir(dir_path.c_str());
    if(!dirp)
    {
        return false;
    }

    struct stat st;
    struct dirent* dp;
    char full_path[256] = { 0 };

    while((dp = readdir(dirp)) != nullptr)
    {
        std::fill_n(full_path, 256, 0);

        if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
        {
            continue;
        }

        std::string full_path = dir_path + dp->d_name;

        int len = strlen(dp->d_name);
        if(len > 6 && strncmp(dp->d_name+(len-6), ".proto", 6) == 0)
        {
            if(import_proto_file(dir_path, dp->d_name) == false)
            {
                closedir(dirp);
                return false;
            }
        }

        if(lstat(full_path.c_str(), &st) < 0)
        {
            continue;
        }

        if(S_ISDIR(st.st_mode))
        {
            if(init_google_protos(full_path) == false)
            {
                closedir(dirp);
                return false;
            }
        }
    }

    closedir(dirp);

    return true;
}

bool MsgCenter::init()
{
    if(init_google_protos(_proto_dir) == false)
    {
        LogError("init google proto files Error");
        return false;
    }

    return true;
}

bool MsgCenter::uinit()
{
    _msg_id_names.clear();

    return true;
}

UserMsgPtr MsgCenter::gen_msg(ProtocolDefine proto_id)
{
    auto mit = _msg_id_names.find(proto_id);
    if(mit == _msg_id_names.end())
    {
        Log("get_msg error 1, proto_id:%d", proto_id);
        return UserMsgPtr();
    }

    UserMsgPtr msg = gen_msg(mit->second);
    if(msg)
    {
        msg->_msg_id = (int)proto_id;
    }

    return msg;
}

UserMsgPtr MsgCenter::gen_msg(int msg_id)
{
    ProtocolDefine proto_id = (ProtocolDefine)msg_id;

    return gen_msg(proto_id);
}

UserMsgPtr MsgCenter::gen_msg(const std::string& msg_name)
{
    using namespace google::protobuf;

    const Descriptor * pDescriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(msg_name);
    if(pDescriptor == nullptr)
    {
        return UserMsgPtr();
    }

    const Message * pPrototype = MessageFactory::generated_factory()->GetPrototype(pDescriptor);
    if (nullptr == pPrototype)
    {
        return UserMsgPtr();
    }

    UserMsgPtr ret_msg = UserMsgPtr(new UserMsg());
    ret_msg->_msg = MsgPtr(pPrototype->New());

    return ret_msg;
}

list<UserMsgPtr> MsgCenter::parse_package(const UserPtr& user)
{
    SocketNodePtr sock = user->_sock;
    list<UserMsgPtr> ret;

    RingBuffPtr sock_buff = sock->_rd_buff;

    while(true)
    {
        if(sock_buff->data_size() <= 4)
        {
            break;
        }

        size_t pkg_len = 0;
        int msg_id = 0;
        int user_id = 0;

        sock_buff->read_begin((char*)&pkg_len, 4);
        if(pkg_len <=1 || pkg_len > 2048)
        {
            LogError("Wrong socket data package len:%ld", pkg_len);
            break;
        }

        // pkg_len + sizeof(pkg_len) + sizeof(msg_id) + sizeof(user_id)
        if(sock_buff->data_size() < pkg_len + 4 + 4 + 4)
        {
            break;
        }

        sock_buff->del_data(4);

        _parse_buff->clean_data();
        _parse_buff->append_size(pkg_len);

        sock_buff->read_del_begin((char*)&msg_id, 4);
        sock_buff->read_del_begin((char*)&user_id, 4);
        sock_buff->read_del_begin(_parse_buff->data(), pkg_len);

        _parse_string = _parse_buff->data();

        UserMsgPtr pmsg = gen_msg(msg_id);
        if(!pmsg)
        {
            LogError("can not gen msg:%d", msg_id);
            break;
        }

        pmsg->_msg->ParseFromString(_parse_string);
        pmsg->_msg_id = msg_id;
        pmsg->_uid = user_id;
        pmsg->_sfd = sock->_sfd;

        ret.push_back(pmsg);
    }

    return ret;
}






