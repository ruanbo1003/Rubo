/*
 * ringbuff_test.cpp
 *
 *      Author: ruanbo
 */


#include "util/RingBuff.hpp"
#include "msgs/protocols/login.pb.h"

#include "comm/Log.hpp"

void ringbuff_1()
{
    RingBuff rb;

    rb.show();

    rb.add_data("1234", 4);


    rb.show();
    rb.add_data("xyz", 3);

    rb.show();


    {
        char rdata[5] = { 0 };
        rb.read_begin(rdata, 4);
        Log("rdata:%s", rdata);
    }

    //rb.del_data(2);

    {
        char rdata2[3] = { 0 };
        rb.read_begin(rdata2, 2);
        Log("rdata2:%s", rdata2);
    }

    rb.step(2);

    rb.show();
}

void ringbuff_2()
{
    Log(" ================= ringbuff_2========");


    RingBuffPtr ring(new RingBuff());

    for(int i=0; i<2; ++i)
    {
        LoginReq msg;
        msg.set_name("robert");
        msg.set_passwd("hello");

        std::string data = "";
        msg.SerializeToString(&data);

        int len = data.length();

        LogLine("data:[");
        for(int d=0; d<len; ++d)
        {
            LogLine("%c", data[d]);
        }
        Log("] data end");

        Log("len[%d]:%d, data:%s", i, len, data.c_str());

        char len_char[4] = { 0 };
        stdcopy(len_char, (char*)&len, 4);

        ring->add_data((char*)&len, 4);
        ring->add_data(data.c_str(), len);

        ring->show();
    }

    for(int i=0; i<2; ++i)
    {
        int len = 0;
        ring->read_begin((char*)&len, 4);
        ring->del_data(4);

        Log("len:%d", len);

        char* msg_buf = new char[len + 1];
        memset(msg_buf, 0, len+1);


        ring->read_begin(msg_buf, len);
        ring->del_data(len);

        ring->show();

        std::string msg_str = msg_buf;

        Log("msg_str:%s", msg_str.c_str())

        LoginReq msg;
        msg.ParseFromString(msg_str);


        delete msg_buf;
    }
}


void ringbuff_3()
{
    Log(" ================= ringbuff_3========");

    RingBuffPtr ring(new RingBuff());

    int len = 9;
    char len_buf[4] = { 0 };
    stdcopy(len_buf, (char*)&len, 4);

    ring->add_data((char*)&len, 4);

    int len2 = 0;
    stdcopy((char*)&len2, len_buf, 4);
    Log("len2:%d", len2);
}

void ringbuff_4()
{
    RingBuffPtr ring(new RingBuff());

    {
        std::string str1 = "hello_1";
        int len1 = str1.length();

        ring->add_data((char*)&len1, 4);
        ring->add_data(str1.c_str(), len1);
        ring->show();

        Log("len1:%d, str1:%s", len1, str1.c_str());
    }

    {
        std::string str2 = "hello_22";
        int len2 = str2.length();

        ring->add_data((char*)&len2, 4);
        ring->add_data(str2.c_str(), len2);
        ring->show();

        Log("len1:%d, str1:%s", len2, str2.c_str());
    }

    {
        int len1 = 0;
        ring->read_begin((char*)&len1, 4);
        ring->del_data(4);

        char* str1 = new char[len1 + 1];
        ring->read_begin(str1, len1);
        ring->del_data(len1);

        str1[len1] = '\0';

        Log("len1:%d, str1:%s", len1, str1);

        delete str1;
    }

    {
        int len2 = 0;
        ring->read_begin((char*)&len2, 4);
        ring->del_data(4);

        char* str2 = new char[len2 + 1];
        ring->read_begin(str2, len2);
        ring->del_data(len2);

        str2[len2] = '\0';

        Log("len1:%d, str1:%s", len2, str2);

        delete str2;
    }

    {
        std::string str3 = "hello_333";
        int len3 = str3.length();

        ring->add_data((char*)&len3, 4);
        ring->add_data(str3.c_str(), len3);
        ring->show();

        Log("len3:%d, str3:%s", len3, str3.c_str());
    }
    {
        int len3 = 0;
        ring->read_begin((char*)&len3, 4);
        ring->del_data(4);

        char* str3 = new char[len3 + 1];
        ring->read_begin(str3, len3);
        ring->del_data(len3);

        str3[len3] = '\0';

        Log("len3:%d, str3:%s", len3, str3);

        delete str3;
    }
}



void ringbuff_tests()
{
//    ringbuff_1();

    ringbuff_2();

//    ringbuff_3();

//    ringbuff_4();
}


