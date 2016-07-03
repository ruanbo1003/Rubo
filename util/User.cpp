/*
 * User.cpp
 *
 *      Author: ruanbo
 */

#include <util/User.hpp>

User::User()
{
    clean();
}

User::~User()
{

}

void User::clean()
{
    _uid = 0;
    _conn_status = CONNECT_NULL;
    if(_sock)
    {
        _sock->release();
    }
}
