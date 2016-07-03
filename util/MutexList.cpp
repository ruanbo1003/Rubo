/*
 * MutexList.cpp
 *
 *      Author: ruanbo
 */

#include <util/MutexList.hpp>



/*
template<class T>
MutexList<T>::MutexList()
{

}

template<class T>
MutexList<T>::~MutexList()
{

}


template<typename T>
void MutexList<T>::add_one(T one)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _datas.push_back(one);
}


template<typename T>
void MutexList<T>::add_more(list<T>& more)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _datas.insert(_datas.end(), more.begin(), more.end());
}


template<typename T>
T MutexList<T>::get_one()
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


template<typename T>
list<T> MutexList<T>::get_more()
{
    std::lock_guard<std::mutex> lock(_mtx);

    list<T> ret;
    ret.swap(_datas);

    return ret;
}

*/













