/*
 * RingBuff.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_RINGBUFF_HPP_
#define UTIL_RINGBUFF_HPP_


#include <comm/global.hpp>
#include <stddef.h>   //size_t

class RingBuff
{
#define DEFAULT_BUFF_SIZE 4

private:
    char *_buff;
    size_t _len;
    size_t _head;
    size_t _tail;

    RingBuff(const RingBuff&) = delete;
    RingBuff& operator = (const RingBuff&) = delete;

public:
    RingBuff();
    virtual ~RingBuff();

private:
    //const char* get_data()const;
    size_t left_size()const;

    bool append_size();
public:
    size_t data_size()const;

    bool del_data(size_t len);
    void step(size_t len);

    bool read_begin(char*dest, size_t len);
    bool read_del_begin(char* dest, size_t len);

    bool add_data(const char* data, size_t len);

    void show()const;
};

using RingBuffPtr = std::tr1::shared_ptr<RingBuff>;

#endif /* UTIL_RINGBUFF_HPP_ */












