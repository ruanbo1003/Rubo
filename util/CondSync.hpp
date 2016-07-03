/*
 * CondSync.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_CONDSYNC_HPP_
#define UTIL_CONDSYNC_HPP_


#include <mutex>
#include <condition_variable>
#include "comm/global.hpp"

class CondSync
{
public:
    std::mutex _mutex;
    std::condition_variable _cond;

public:
    CondSync();
    virtual ~CondSync();

public:
    void notify_one();
    void notify_all();

};

using CondSyncPtr = std::tr1::shared_ptr<CondSync>;



#endif /* UTIL_CONDSYNC_HPP_ */
