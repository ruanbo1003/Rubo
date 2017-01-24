/*
 * Timer.cpp
 *
 *  Created on: Jan 24, 2017
 *      Author: ruanbo
 */

#include <util/Timer.hpp>

Timer::Timer(int32_t interval, bool repeat)
{
    _repeat = repeat;
    _interval = interval;
}

Timer::~Timer()
{
    // TODO Auto-generated destructor stub
}

Timers::Timers()
{

}


Timers::~Timers()
{

}
