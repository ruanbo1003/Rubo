/*
 * Time.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_TIME_HPP_
#define UTIL_TIME_HPP_

#include "comm/Log.hpp"
#include <chrono>
using namespace std;


class Time
{
    time_t _time;

public:
    Time();
    virtual ~Time();
};


class DeltaTime
{
private:
    long int _begin;
    long int _end;

public:
    DeltaTime()
    {
        _begin = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    }

    virtual ~DeltaTime()
    {
        _end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        auto delta_ms = _end - _begin;

        Log("delta time:%ld.%ld", delta_ms/1000, delta_ms%1000);
    }
};

#endif /* UTIL_TIME_HPP_ */
