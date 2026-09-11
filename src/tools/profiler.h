/**
 * ---------- profiler.h ----------------
 *
 * Declares profiler logic.
 *
 * --------------------------------------
 */

#if _INCLUDE_PROFILER

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "tools/time.h"

// ------------- MACROS -----------------

#define fl_prof_profile_once(segment) fl_prof_profile(1, 1, "", segment)

#define fl_prof_profile(samples, input_size, input_unit, segment)     \
    {                                                                 \
        fl_double *ms = fl_mem_alloc_arr(samples, sizeof(fl_double)); \
                                                                      \
        for (fl_uint i = 0; i < samples; i++)                         \
        {                                                             \
            fl_long __start = fl_time_get_ticks();                    \
            segment;                                                  \
            fl_long __end = fl_time_get_ticks();                      \
            ms[i] = fl_time_ticks_diff(__start, __end);               \
        }                                                             \
        _fl_prof_log(                                                 \
            #segment, _PROFILER_LOGS_PATH,                            \
            input_unit, input_size, ms, samples);                     \
        fl_mem_free(ms);                                              \
    }

// ---------- DECLARATIONS --------------

fl_void _fl_prof_log(
    const fl_char *what,
    const fl_char *filename,
    const fl_char *input_unit,
    fl_size input_size,
    fl_double *measures,
    fl_size measures_count);

#else

#define fl_prof_profile(samples, input_size, input_unit, segment) segment

#endif