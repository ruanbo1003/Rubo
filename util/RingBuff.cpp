/*
 * RingBuff.cpp
 *
 *      Author: ruanbo
 */

#include "RingBuff.hpp"

#include "comm/global.hpp"
#include "comm/Log.hpp"

RingBuff::RingBuff()
{
    _head = 0;
    _tail = 0;
    _len = DEFAULT_BUFF_SIZE;
    _buff = new char[_len];
    bzero(_buff, _len);
}

RingBuff::~RingBuff()
{
    delete [] _buff;
}

size_t RingBuff::left_size()const
{
    if(_tail <= _head)
    {
        return _len - (_head - _tail);
    }
    else
    {
        return (_tail - _head);
    }
}

size_t RingBuff::data_size()const
{
    return _len - left_size();
}

bool RingBuff::append_size()
{
    size_t new_len = _len > 1024? (_len + DEFAULT_BUFF_SIZE) : (_len * 2);
    char* new_buff = new char[new_len];
    bzero(new_buff, new_len);

    size_t data_len = data_size();

    if(_tail <= _head)
    {
        stdcopy(new_buff, _buff+_tail, (_head - _tail));
    }
    else
    {
        size_t len1 = _len - _tail;
        stdcopy(new_buff, _buff+_tail, len1);
        stdcopy(new_buff+len1, _buff, _head);
    }

    _head = data_len;
    _tail = 0;
    _len = new_len;
    delete [] _buff;
    _buff = new_buff;

    return true;
}

bool RingBuff::del_data(size_t len)
{
    if(data_size() < len || len == 0)
    {
        return false;
    }


    if(_head > _tail)
    {
        std::fill_n(_buff+_tail, len, 0);
    }
    else if(_head < _tail)
    {
        size_t len1 = (_len - _tail);
        if(len1 >= len)
        {
            std::fill_n(_buff+_tail, len, 0);
        }
        else
        {
            std::fill_n(_buff+_tail, len1, 0);
            std::fill_n(_buff, len-len1, 0);
        }
    }
    else
    {
        LogError("_head == _tail");
    }

    _tail = (_tail + len) % _len;


    return true;
}

void RingBuff::step(size_t len)
{
    _tail = (_tail + len) % _len;
}

bool RingBuff::read_begin(char*dest, size_t len)
{
//    Log("read_begin, _len:%ld, _head:%ld,_tail:%ld", _len, _head, _tail);

    if(data_size() < len)
    {
        return false;
    }

    if(_head >= _tail)
    {
        stdcopy(dest, _buff+_tail, len);
    }
    else
    {
        size_t len1 = _len - _tail;
        if(len1 >= len)
        {
            stdcopy(dest, _buff+_tail, len);
        }
        else
        {
            stdcopy(dest, _buff+_tail, len1);
            stdcopy(dest+len1, _buff, len-len1);
        }
    }

    return true;
}

bool RingBuff::read_del_begin(char* dest, size_t len)
{
    if(data_size() < len)
    {
        return false;
    }

    if(_head >= _tail)
    {
        stdcopy(dest, _buff+_tail, len);
    }
    else
    {
        size_t len1 = _len - _tail;
        if(len1 >= len)
        {
            stdcopy(dest, _buff+_tail, len);
        }
        else
        {
            stdcopy(dest, _buff+_tail, len1);
            stdcopy(dest+len1, _buff, len-len1);
        }
    }

    return del_data(len);
}

bool RingBuff::add_data(const char* data, size_t len)
{
    while(left_size() <= len + 1)
    {
        append_size();
    }

    if(_head >= _tail)
    {
        if(_len - _head >= len)
        {
            stdcopy(_buff + _head, data, len);
            _head = (_head + len)%_len;
        }
        else
        {
            size_t len1 = _len - _head;
            stdcopy(_buff + _head, data, len1);
            stdcopy(_buff, data+len1, (len - len1));
            _head = len - len1;
        }
    }
    else
    {
        stdcopy(_buff + _head, data, len);
        _head = (_head + len)%_len;
    }

    return true;
}

void RingBuff::show()const
{
    Log("RingBuff data, len:%d:", (int)_len);

    for(size_t i=0; i<_len; ++i)
    {
        if(i==_head && i==_tail)
        {
            LogLine("%d(ht) ", (int)i);
        }
        else if(i == _head)
        {
            LogLine("%d(h) ", (int)i);
        }
        else if(i == _tail)
        {
            LogLine("%d(t) ", (int)i);
        }
        else
        {
            LogLine("%d ", (int)i);
        }
    }

    Log(" ");

    for(size_t i=0; i<_len; ++i)
    {
        if(i==_head && i==_tail)
        {
            LogLine("%c(ht) ", _buff[i]);
        }
        else if(i == _head)
        {
            LogLine("%c(h) ", _buff[i]);
        }
        else if(i == _tail)
        {
            LogLine("%c(t) ", _buff[i]);
        }
        else
        {
            LogLine("%c ", _buff[i]);
        }
    }
    Log(" ");
    Log("RingBuff show end");

}















