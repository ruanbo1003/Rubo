/*
 * time_test.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: ruanbo
 */

#include "comm/global.hpp"
#include <chrono>


void time_test_1()
{

    auto now = chrono::system_clock::now();
//    long int cur_ms = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now());
    time_t nowt = chrono::system_clock::to_time_t(now);

    Log("nowt:%d", nowt);

    long int ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();
    Log("ms:%ld", ms);

    long int min = chrono::duration_cast<chrono::minutes >(now.time_since_epoch()).count();
    Log("min:%ld", min);

    std::tm tm = *std::localtime(&nowt);
    Log("%d.%d.%d %d:%d:%d", tm.tm_year+1900, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    Log("ctime:%s", std::ctime(&nowt));
}


void timer_test_1()
{
    auto now = chrono::system_clock::now();

    time_t last_s = chrono::system_clock::to_time_t(now);
//    long int last_ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();

    int scnt = 0;

    while(scnt <= 10)
    {
        auto now = chrono::system_clock::now();
        time_t nows = chrono::system_clock::to_time_t(now);

        if(nows > last_s)
        {
            scnt++;
            last_s = nows;

            Log("on next second");
        }
    }
}


void time_tests()
{
    time_test_1();

//    timer_test_1();

}


