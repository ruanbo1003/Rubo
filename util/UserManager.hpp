/*
 * ProcessCentre.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_USERMANAGER_HPP_
#define UTIL_USERMANAGER_HPP_

#include <comm/global.hpp>
#include <map>

#include "User.hpp"
class UserManager
{
public:
    std::map<int, UserPtr> _uid_users;    // 只有登陆了才会加入到里面
    std::map<int, UserPtr> _sock_users;   // 只要连接就会加入到这个里面

public:
    UserManager();
    virtual ~UserManager();

public:
    bool add_user(const UserPtr& user);
    bool del_user(int uid);

    // 客户端连接到服务端，但还没有进行登陆操作, uid=0
    bool add_conn(const UserPtr& user);
    SocketNodePtr get_conn(int sfd);
    void del_conn(int sfd);

public:
    UserPtr get_user_by_sock(int sock_fd);
    UserPtr get_user_by_uid(int uid);
};


using UserManagerPtr = std::tr1::shared_ptr<UserManager>;


#endif /* UTIL_USERMANAGER_HPP_ */
