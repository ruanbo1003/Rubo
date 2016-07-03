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
}

IThread::~IThread()
{

}


bool IThread::thread_func()
{
    while(is_running())
    {
        thread_loop();
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







