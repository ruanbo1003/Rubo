/*
 * CondSync.cpp
 *
 *      Author: ruanbo
 */

#include "CondSync.hpp"

CondSync::CondSync()
{

}

CondSync::~CondSync()
{

}

void CondSync::notify_one()
{
    _cond.notify_one();
}

void CondSync::notify_all()
{
    _cond.notify_all();
}
