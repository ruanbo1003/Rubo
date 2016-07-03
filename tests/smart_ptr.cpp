/*
 * smart_ptr.cpp
 *
 *      Author: ruanbo
 */


#include "comm/Log.hpp"

#include <tr1/memory>
#include <tr1/shared_ptr.h>


class UserTest
{
public:
    UserTest()
    {
        Log("UserTest()");
    }
    virtual ~UserTest()
    {
        Log("~UserTest()");
    }

public:
    void show()
    {
        Log("im a user");
    }
};

using UserTestPtr = std::tr1::shared_ptr<UserTest>;


class UD : public UserTest
{
public:
    UD()
    {
        Log("UD()");
    }
    virtual ~UD()
    {
        Log("~UD()");
    }
};
using UDPtr = std::tr1::shared_ptr<UD>;

UserTestPtr getPtr(int type)
{
    if(type ==1 )
    {
        return UserTestPtr(new UserTest());
    }
    else
    {
        return UserTestPtr();
    }
}

void smart_ptr_equal_test()
{
    UserTestPtr p1(new UserTest());
    UserTestPtr p2 = p1;

    Log("p1 == p2:%d", p1 == p2);

    UserTestPtr p3();
    Log("p3 == nullptr:%d", p3 == nullptr);

    p2.reset();
    Log("p2 == nullptr:%d", p2 == nullptr);

    UserTestPtr p4 = getPtr(1);
    UserTestPtr p5 = getPtr(2);

    if(p4)
    {
        Log("if p4");   // true
    }
    if(p5)
    {
        Log("if p5");
    }

}

void smart_ptr_tests()
{

    if(false)
    {
        UserTestPtr sp(new UserTest());

        sp->show();
        Log("user count:%ld", sp.use_count());

        {
            UserTestPtr sp2 = sp;
            Log("user count:%ld", sp.use_count());
        }

        Log("user count:%ld", sp.use_count());

        sp.reset();

        Log("user count:%ld", sp.use_count());
    }

    if(false)
    {
        UserTestPtr utp(new UD());
    }

    if(false)
    {
        static auto my_free = [](void* data){free(data);};

        char* buff = (char*)malloc(64);
        std::tr1::shared_ptr<char> cp(buff, my_free);

        char* buff2 = (char*)malloc(128);
        cp.reset(buff2, my_free);
    }

    if(false)
    {
        smart_ptr_equal_test();
    }

    if(true)
    {
        UDPtr d = UDPtr(new UD());

        UserTestPtr b = d;

        UDPtr d1 = std::tr1::static_pointer_cast<UD>(b);
        if(d1)
        {
            Log("static_pointer_case ok");
        }
    }

}





