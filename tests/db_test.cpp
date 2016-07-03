/*
 * db_test.cpp
 *
 *      Author: ruanbo
 */


#include <comm/global.hpp>
#include <mysql.h>
#include <stddef.h>

#include "server/DB/MysqlDb.hpp"
#include "util/Time.hpp"

class MysqlDbTest
{
public:
    MYSQL _mysql;

public:
    MysqlDbTest()
    {
        mysql_init(&_mysql);
    }

    virtual ~MysqlDbTest()
    {
        mysql_close(&_mysql);
    }


public:
    void conn_test()
    {

    }
};


void db_tests()
{
    DeltaTime dt;

    MysqlDbPtr mysql_db = MysqlDbPtr(new MysqlDb());

    if(mysql_db->init() == false)
    {
        LogError("mysqldb init error");
        return;
    }

    if(false)
    {
        string name = "robert";
        for(int i=0; i<2; ++i)
        {
            bool ret = mysql_db->check_name_registered(name);

            Log("check name register, ret:%d", ret);
        }
    }

    if(false)
    {
        mysql_db->register_user("robert", "123456", 110);
    }
}

