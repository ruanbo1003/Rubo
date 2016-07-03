/*
 * MutexList.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_MUTEXLIST_HPP_
#define UTIL_MUTEXLIST_HPP_


#include "comm/global.hpp"
#include <mutex>

template<class T>
class MutexList
{
private:
    std::mutex _mtx;
    list<T> _datas;

public:
    MutexList()
    {

    }
    virtual ~MutexList()
    {

    }

public:
    void add_one(T one)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _datas.push_back(one);
    }
    void add_more(const list<T>& more)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _datas.insert(_datas.end(), more.begin(), more.end());
    }

    T get_one()
    {
        std::lock_guard<std::mutex> lock(_mtx);

        if(_datas.size() == 0)
        {
            return T();
        }

        T one = _datas.front();
        _datas.pop_front();

        return one;
    }

    list<T> get_more()
    {
        std::lock_guard<std::mutex> lock(_mtx);

        list<T> ret;
        ret.swap(_datas);

        return ret;
    }

};

template<class T>
using MutexListPtr = tr1::shared_ptr<MutexList<T>>;


#endif /* UTIL_MUTEXLIST_HPP_ */
















