/*
 * Log.cpp
 *
 *      Author: ruanbo
 */

#include "Log.hpp"

void LogDebug(const char* fmt, ...)
{
#ifdef DEBUG_MODE
    va_list args;
    va_start(args, fmt);

//    printf(fmt, *args);
    vprintf(fmt, args);

    va_end(args);

    printf("\n");
 #endif
}
