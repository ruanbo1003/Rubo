/*
 * LineBuffer.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_LINEBUFFER_HPP_
#define UTIL_LINEBUFFER_HPP_


#include <comm/global.hpp>

class LineBuffer
{
#define DEF_LINE_BUFF_SIZE 256

private:
    size_t _buff_size;
    std::tr1::shared_ptr<char> _buff;


public:
    LineBuffer();
    virtual ~LineBuffer();


private:
    static void my_free(void *data);

public:
    void append_size(size_t need_size = 0);

    void clean_data();

    size_t buff_size();

    char* data();

};

using LineBufferPtr = std::tr1::shared_ptr<LineBuffer>;



#endif /* UTIL_LINEBUFFER_HPP_ */
