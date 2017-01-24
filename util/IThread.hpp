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
#include <chrono>

class IThread : public NonCopyable
{
private:
    std::thread _thread;
    bool _quit;
    bool _over;  // 是否已经退出了
    std::string _name;

    //定时器变量
    long int _last_ms;
    long int _last_s;
    std::tm _last_tm;

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

private:
    void init_time();
    void timer_events();
public:
    virtual void on_next_second();
    virtual void on_next_minute();
    virtual void on_next_hour();
};

using ThreadPtr = std::tr1::shared_ptr<IThread>;


#endif /* UTIL_ITHREAD_HPP_ */
