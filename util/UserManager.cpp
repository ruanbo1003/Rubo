/*
 * ProcessCentre.cpp
 *
 *      Author: ruanbo
 */

#include <util/UserManager.hpp>

UserManager::UserManager()
{
    Log("UserManager::UserManager");
}

UserManager::~UserManager()
{
    Log("UserManager::~UserManager");

    _uid_users.clear();
    _sock_users.clear();
}


bool UserManager::add_user(const UserPtr& user)
{
    auto uit = _uid_users.find(user->_uid);
    if(uit == _uid_users.end() && user->_uid > 0)
    {
        _uid_users[user->_uid] = user;
    }

    // SocketNode
    if(user->_sock)
    {
        auto sit = _sock_users.find(user->_sock->_sfd);
        if(sit == _sock_users.end() && user->_sock->_sfd > 0)
        {
            _sock_users[user->_sock->_sfd] = user;
        }
    }

    return true;
}

bool UserManager::add_conn(const UserPtr& user)
{
    auto sit = _sock_users.find(user->_sock->_sfd);
    if(sit == _sock_users.end() && user->_sock->_sfd > 0)
    {
        _sock_users[user->_sock->_sfd] = user;
    }

    auto uit = _uid_users.find(user->_uid);
    if(uit == _uid_users.end() && user->_uid > 0)
    {
        _uid_users[user->_uid] = user;
    }

    return true;
}

SocketNodePtr UserManager::get_conn(int sid)
{
    auto sit = _sock_users.find(sid);
    if(sit == _sock_users.end())
    {
        return SocketNodePtr();
    }

    return sit->second->_sock;
}

void UserManager::del_conn(int sfd)
{
    auto sit = _sock_users.find(sfd);
    if(sit != _sock_users.end())
    {
        UserPtr user = sit->second;
        _sock_users.erase(sit);

        auto uit = _uid_users.find(user->_uid);
        if(uit != _uid_users.end())
        {
            _uid_users.erase(uit);
        }
    }
}

bool UserManager::del_user(int uid)
{
    auto uit = _uid_users.find(uid);
    if(uit != _uid_users.end())
    {
        UserPtr user = uit->second;
        _uid_users.erase(uit);

        if(user->_sock)
        {
            auto sit = _sock_users.find(user->_sock->_sfd);
            if(sit != _sock_users.end())
            {
                _sock_users.erase(sit);
            }
        }
    }
    return true;
}

UserPtr UserManager::get_user_by_sock(int sock_fd)
{
    auto sit = _sock_users.find(sock_fd);
    if(sit != _sock_users.end())
    {
        return sit->second;
    }

    return UserPtr();
}

UserPtr UserManager::get_user_by_uid(int uid)
{
    auto uit = _uid_users.find(uid);
    if(uit != _uid_users.end())
    {
        return uit->second;
    }

    return UserPtr();
}














