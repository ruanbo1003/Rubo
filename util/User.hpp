/*
 * User.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_USER_HPP_
#define UTIL_USER_HPP_

#include <comm/global.hpp>
#include "SocketNode.hpp"

enum ConnStatus
{
    CONNECT_NULL = 0,
    CONNECT_CONNECTING = 1,
    CONNECT_LOGINED = 2,
    CONNECT_CLOSING = 3,

    CONNECT_END
};

class User
{
public:
    int _uid;
    ConnStatus _conn_status;
    SocketNodePtr _sock;

public:
    User();
    virtual ~User();

public:
    void clean();
};

using UserPtr = std::tr1::shared_ptr<User>;


#endif /* UTIL_USER_HPP_ */
