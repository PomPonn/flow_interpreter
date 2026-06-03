/**
 * ---------- time.win.c ----------------
 *
 * Implements Windows platform
 * time manipulation logic.
 *
 * --------------------------------------
 */

#ifdef _WIN32

// ---------- DEPENDENCIES --------------

#include "time.h"

#include <windows.h>

// ---------- DEFINITIONS ---------------

fl_long fl_time_get_ticks()
{
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    return ticks.QuadPart;
}

// returns number of microseconds
fl_double fl_time_ticks_diff(fl_long start, fl_long end)
{
    static LARGE_INTEGER frequency = {.QuadPart = 0};

    if (!frequency.QuadPart)
        QueryPerformanceFrequency(&frequency);

    return (end - start) * 1.0e6 / frequency.QuadPart;
}

#endif
