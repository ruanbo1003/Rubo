/*
 * MysqlDb.cpp
 *
 *      Author: ruanbo
 */

#include <server/DB/MysqlDb.hpp>
#include "util/utils.hpp"

#include <fstream>

MysqlDb::MysqlDb() : _driver(nullptr)
{
    _db_host = "";
    _passwd = "";
    _db_name = "";
}

MysqlDb::~MysqlDb()
{
    Log("MysqlDb::~MysqlDb");

    unit();
}

bool MysqlDb::init()
{
    _driver = get_driver_instance();
    if(!_driver)
    {
        LogError("MySql get_driver_instance() error");
        return false;
    }

    _conn = MysqlConnPtr(_driver->connect("127.0.0.1:3306", "root", "123456"));
    if(!_conn)
    {
        LogError("mysqlcppconn connect to mysql error");
        unit();
        return false;
    }

    if(_conn->isValid() == false)
    {
        LogError("mysqlcppconn is not valid");
        unit();
        return false;
    }

    _conn->setSchema("users");
    _conn->setAutoCommit(true);

    _stmt = MysqlStatementPtr(_conn->createStatement());
    if(!_stmt)
    {
        LogError("MySqlDb createStatement() error");
        unit();
        return false;
    }

    if(init_sql() == false)
    {
        unit();
        return false;
    }

    return true;
}




void MysqlDb::unit()
{
    if(_conn)
    {
        _conn->close();
    }

    if(_driver)
    {
        _driver = nullptr;
    }
}

bool MysqlDb::init_sql()
{
    std::ifstream ifs("server/DB/mysql.sql", std::ifstream::in);
    if(ifs.is_open() == false)
    {
        LogError("open file mysql.sql error");
        return false;
    }

    string file_sql((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    ifs.close();


    bool ret = false;
    std::vector<string> sqls;
    utils::split(file_sql, ";", sqls);
    for(auto& one_sql : sqls)
    {
        if(one_sql.length() < 5)
        {
            continue;
        }
        utils::trim_left(one_sql, '\n');

        ret = execute_sql(one_sql);
        if(ret == false)
        {
            LogError("execute sql: \n%s error!!!!", one_sql.c_str());
            return false;
        }
    }

    return true;
}

bool MysqlDb::execute_sql(const string& sql)
{
    try
    {
        _stmt->execute(sql);

        return true;
    }
    catch (sql::SQLException& e)
    {
        LogError("execute sql: \n%s \n SQLException:%s", sql.c_str(), e.what());
        return false;
    }
    catch(std::exception& e)
    {
        LogError("execute sql: \n%s \n std::exception:%s", sql.c_str(), e.what());
        return false;
    }
}

MysqlResPtr MysqlDb::execute_query(const string& query_sql)
{
    try
    {
//        MysqlPreStatePtr prestate = MysqlPreStatePtr(_conn->prepareStatement(query_sql));
//        return MysqlResPtr(prestate->executeQuery());
        return MysqlResPtr(_stmt->executeQuery(query_sql));
    }
    catch (sql::SQLException& e)
    {
        LogError("SQL error: %s", query_sql.c_str());
        return MysqlResPtr();
    }
    catch(std::exception& e)
    {
        LogError("std error: %s", query_sql.c_str());
        return MysqlResPtr();
    }
}

int MysqlDb::execute_pre_update(const MysqlPreStatePtr& pre_stat)
{
    try
    {
        return pre_stat->executeUpdate();
    }
    catch (sql::SQLException& e)
    {
        LogError("sql::SQLException:mysql prestatement update error");
        return -1;
    }
    catch(std::exception& e)
    {
        LogError("std::exception:mysql prestatement update error");
        return -1;
    }

}


bool MysqlDb::check_name_registered(const string& name)
{
    static string check_name_pat = "Select uid From register Where name='";
    string check_name_sql =  check_name_pat + name + "';";

    MysqlResPtr res = execute_query(check_name_sql);

    if(res)
    {
        return res->rowsCount() >= 1;
    }
    else
    {
        return false;
    }
}

bool MysqlDb::register_user(const string& name, const string& passwd, time_t time)
{
    static string register_pat = "Insert Into register(name,passwd,time) Values(?,?,?)";

    _pstmt = MysqlPreStatePtr(_conn->prepareStatement(register_pat));

    _pstmt->setString(1, name);
    _pstmt->setString(2, passwd);
    _pstmt->setInt(3, time);

//    _pstmt->executeUpdate();
    int update_ret = execute_pre_update(_pstmt);

    return update_ret > 0;
}

int MysqlDb::get_uid(const string& name, const string& passwd)
{
    static string get_sql_format = "Select uid From register Where name='%s' And passwd='%s'";
    string get_sql = string_format(get_sql_format, name.c_str(), passwd.c_str());

    Log("get_uid_sql:%s", get_sql.c_str());


    MysqlResPtr res = execute_query(get_sql);
    if(!res || res->rowsCount() <= 0)
    {
        return -1;
    }

    res->next();
    Log("get_uid_sql res rowsCount:%d", res->rowsCount());

    return res->getInt("uid");
}












