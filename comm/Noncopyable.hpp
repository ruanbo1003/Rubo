/*
 * Noncopyable.hpp
 *
 *      Author: ruanbo
 */

#ifndef COMM_NONCOPYABLE_HPP_
#define COMM_NONCOPYABLE_HPP_


class NonCopyable
{
public:
    NonCopyable() = default;

private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator = (const NonCopyable&) = delete;
};




#endif /* COMM_NONCOPYABLE_HPP_ */
