/*
 * template_head.hpp
 *
 *      Author: ruanbo
 */

#ifndef TESTS_TEMPLATE_HEAD_HPP_
#define TESTS_TEMPLATE_HEAD_HPP_

#include "comm/global.hpp"
#include <iostream>


template<class T>
class TA
{
private:
    T _data;

public:
    TA()
    {
        cout << "TA()" << endl;
    }

    ~TA()
    {
        cout << "~TA()" << endl;
    }

    void set(int i)
    {
        cout <<"TA set:" << i << endl;
    }

    void add(const T& t)
    {
        cout << "add" << endl;
        if(t)
        {
            cout << "t is ok" << endl;
        }
        _data = t;
        _data->show();
    }

};
template<class T>
using TAPtr = std::tr1::shared_ptr<TA<T>>;

class SItem
{
public:
    SItem()
    {
        cout << "SItem()" << endl;
    }
    ~SItem()
    {
        cout << "~SItem()" << endl;
    }

    void show()
    {
        cout << "I am a item" << endl;
    }
};
using SItemPtr = std::tr1::shared_ptr<SItem>;






#endif /* TESTS_TEMPLATE_HEAD_HPP_ */
