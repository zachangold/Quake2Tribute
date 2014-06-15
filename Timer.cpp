//---------------------------------------------------------------------------

#pragma hdrstop

#include "Timer.h"

/**
 * Unused
 */
void Timer::start() {
    GetSystemTime( &startTime );
};

/**
 * Returns the system time in milliseconds.
 */
unsigned int Timer::getTimeMillis() {
    // ask windows for the system time, storing it in currentTime
    GetSystemTime( &currentTime );

    // calculate the millisecond number
    return (currentTime.wMilliseconds - startTime.wMilliseconds) +
            (currentTime.wSecond - startTime.wSecond) * 1000 +
            (currentTime.wMinute - startTime.wMinute) * 1000 * 60 +
            (currentTime.wHour - startTime.wHour) * 1000 * 60 * 60 +
            (currentTime.wDay - startTime.wDay) * 1000 * 60 * 60 * 24;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
 