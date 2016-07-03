/*
 * IThread.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_ITHREAD_HPP_
#define UTIL_ITHREAD_HPP_

#include <comm/global.hpp>
#include "comm/Noncopyable.hpp"
#include <thread>
#include <mutex>


class IThread : public NonCopyable
{
private:
    std::thread _thread;
    bool _quit;
    bool _over;  // 是否已经退出了
    std::string _name;

public:
    IThread(const std::string& name="");
    virtual ~IThread();

public:
    void tover();
    bool is_over();
    bool is_running();
    void tsleep(int n);
    void tmsleep(int n);

public:
    bool thread_func();
    virtual bool init() = 0;
    virtual void unit() = 0;
    virtual void thread_loop() = 0;

    virtual bool start();
    virtual bool stop();
    virtual bool join();

public:
    void set_tname(const string& name);
    string get_name()const;
    const char* get_cname()const;
};

using ThreadPtr = std::tr1::shared_ptr<IThread>;


#endif /* UTIL_ITHREAD_HPP_ */
