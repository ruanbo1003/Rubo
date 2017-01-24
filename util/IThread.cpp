/*
 * IThread.cpp
 *
 *      Author: ruanbo
 */

#include "IThread.hpp"
#include "comm/Log.hpp"



IThread::IThread(const std::string& name)
{
    _quit = false;
    _name = name;
    _over = true;

    _last_ms = 0;
    _last_s = 0;
    _last_tm = { 0 };
}

IThread::~IThread()
{

}


bool IThread::thread_func()
{
    while(is_running())
    {
        thread_loop();

        timer_events();
    }

    unit();
    tover();

    return true;
}

bool IThread::start()
{
    if(is_running() == false)
    {
        return true;
    }

    LogDebug("thread %s start", _name.c_str());

    init_time();

    _over = false;
    _thread = std::thread(std::bind(&IThread::thread_func, this));

    return true;
}

bool IThread::stop()
{
    _quit = true;

    return true;
}

bool IThread::join()
{
    _thread.join();

    LogDebug("IThread[%s] stop done", _name.c_str());

    return true;
}
void IThread::tover()
{
    _over = true;
}

bool IThread::is_over()
{
    return _over;
}

bool IThread::is_running()
{
    return (_quit == false);
}

void IThread::tsleep(int n)
{
    std::this_thread::sleep_for(std::chrono::seconds(n));
}

void IThread::tmsleep(int n)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(n));
}

void IThread::set_tname(const string& name)
{
    _name = name;
}

string IThread::get_name()const
{
    return _name;
}

const char* IThread::get_cname()const
{
    return _name.c_str();
}


void IThread::init_time()
{
    auto nowt = chrono::system_clock::now();
    _last_ms = chrono::duration_cast<chrono::milliseconds>(nowt.time_since_epoch()).count();
    _last_s = chrono::system_clock::to_time_t(nowt);
    _last_tm = *std::localtime(&_last_s);
}

void IThread::timer_events()
{
    auto nowt = chrono::system_clock::now();
    long int cur_ms = chrono::duration_cast<chrono::milliseconds>(nowt.time_since_epoch()).count();
    long int cur_s = chrono::system_clock::to_time_t(nowt);
    std::tm cur_tm = *std::localtime(&cur_s);

    bool need_update = false;
    if(cur_tm.tm_sec != _last_tm.tm_sec)
    {
        need_update = true;
        on_next_second();
    }
    if(cur_tm.tm_min != _last_tm.tm_min)
    {
        need_update = true;
        on_next_minute();
    }
    if(cur_tm.tm_hour != _last_tm.tm_hour)
    {
        need_update = true;
        on_next_hour();
    }

    if(need_update == true)
    {
        _last_tm = cur_tm;
    }

    if(cur_s - _last_s > 1)
    {
        Log("定时间隔超过1秒。");
    }

    _last_s = cur_s;
    _last_ms = cur_ms;
}

void IThread::on_next_second()
{
//    Log("IThread::on_next_second");
}
void IThread::on_next_minute()
{
//    Log("IThread::on_next_minute");
}

void IThread::on_next_hour()
{
//    Log("IThread::on_next_hour");
}





