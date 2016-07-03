/*
 * redis_test.cpp
 *
 *      Author: ruanbo
 */


#include <stdio.h>
#include<hiredis/hiredis.h>

#include "comm/Log.hpp"

#include <iostream>
using namespace std;





void reids_tests()
{
//    redisContext *conn = redisConnect("127.0.0.1", 6379);
    struct timeval conn_tv = {3, 0};
    redisContext *conn = redisConnectWithTimeout("127.0.0.1", 6379, conn_tv);
    if(conn == nullptr)
    {
        Log("connect to redis failed");
        return;
    }
    if(conn->err)
    {
        Log("connect to redis failed:%s", conn->errstr);
        return;
    }

    Log("connect to redis ok");

    redisReply* reply = (redisReply*)redisCommand(conn, "set foo bar");
    freeReplyObject(reply);


    reply = (redisReply*)redisCommand(conn, "get foo");

    Log("get foo:%s", reply->str);

    redisFree(conn);
}

