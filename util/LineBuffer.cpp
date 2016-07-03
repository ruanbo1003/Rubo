/*
 * LineBuffer.cpp
 *
 *      Author: ruanbo
 */

#include <util/LineBuffer.hpp>
#include <string.h>

LineBuffer::LineBuffer()
{
    char* buff = (char*)malloc(DEF_LINE_BUFF_SIZE);
    std::fill_n(buff, DEF_LINE_BUFF_SIZE, 0);

    _buff_size = DEF_LINE_BUFF_SIZE;
    _buff = std::tr1::shared_ptr<char>(buff, my_free);
}

LineBuffer::~LineBuffer()
{

}

void LineBuffer::my_free(void *data)
{
    free(data);
}


void LineBuffer::append_size(size_t need_size)
{
    size_t new_size = 0;
    if(need_size == 0)
    {
        if(_buff_size >= 1024 * 512)
        {
            new_size = _buff_size + 1024 * 4;
        }
        else
        {
            new_size = _buff_size * 2;
        }
    }
    else
    {
        new_size = _buff_size;
        while(true)
        {
            if(new_size < need_size)
            {
                new_size = new_size * 2;
            }
            else
            {
                break;
            }
        }
    }


    char* new_buff = (char*)malloc(new_size);
    std::fill_n(new_buff, new_size, 0);

    std::copy(_buff.get(), _buff.get()+_buff_size, new_buff);
    _buff_size = new_size;
    _buff.reset(new_buff, my_free);
}


void LineBuffer::clean_data()
{
    std::fill_n(_buff.get(), _buff_size, 0);
}

size_t LineBuffer::buff_size()
{
    return _buff_size;
}

char* LineBuffer::data()
{
    return _buff.get();
}











