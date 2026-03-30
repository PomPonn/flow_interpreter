#ifdef _WIN32
#include "time.h"

#include <windows.h>

fl_timestamp fl_time_get()
{
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    return ticks.QuadPart;
}

// returns number of microseconds
fl_double fl_time_diff(fl_timestamp start, fl_timestamp end)
{
    static LARGE_INTEGER frequency = {.QuadPart = 0};

    if (!frequency.QuadPart)
    {
        QueryPerformanceFrequency(&frequency);
    }

    return (end - start) * 1.0e6 / frequency.QuadPart;
}

#endif