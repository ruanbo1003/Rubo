/*
 * Timer.hpp
 *
 *  Created on: Jan 24, 2017
 *      Author: ruanbo
 */

#ifndef UTIL_TIMER_HPP_
#define UTIL_TIMER_HPP_

#include "comm/global.hpp"
#include "comm/Noncopyable.hpp"
#include "stdint.h"

class Timer : public NonCopyable
{
private:
    bool _repeat;      // 是否是重复的
    int32_t _interval;   //定时时间间隔

public:
    Timer(int32_t interval, bool repeat=false);
    virtual ~Timer();
};

using TimerPtr = std::tr1::shared_ptr<Timer>;



class Timers : public NonCopyable
{
private:
    list<TimerPtr> _timers;

    Timers();
    virtual ~Timers();
public:
    void addTimer();

    TimerPtr getTimer();

};





#endif /* UTIL_TIMER_HPP_ */
