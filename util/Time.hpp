/*
 * Time.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_TIME_HPP_
#define UTIL_TIME_HPP_

#include <comm/global.hpp>
#include <sys/time.h>
//#include <time.h>

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
    struct timeval _begin;
    struct timeval _end;

public:
    DeltaTime()
    {
        gettimeofday(&_begin, nullptr);
    }

    virtual ~DeltaTime()
    {
        gettimeofday(&_end, nullptr);

        Log("delta time:%ld.%ldS", (_end.tv_sec-_begin.tv_sec), (_end.tv_usec-_begin.tv_usec)/1000);
    }
};

#endif /* UTIL_TIME_HPP_ */
