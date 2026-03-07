#pragma once

#include "shared/common.h"
#include "shared/memory.h"
#include "tools/time.h"

#define fl_prof_profile(samples, input_size, input_label, segment)    \
    {                                                                 \
        fl_double *ms = fl_mem_alloc_arr(samples, sizeof(fl_double)); \
                                                                      \
        for (fl_uint i = 0; i < samples; i++)                         \
        {                                                             \
            fl_timestamp __start = fl_time_get();                     \
            segment;                                                  \
            ms[i] = fl_time_diff(__start, fl_time_get()) * 1e6;       \
        }                                                             \
        _fl_prof_log(                                                 \
            #segment, _PROFILER_LOGS_PATH,                            \
            input_label, input_size, ms, samples);                    \
    }

fl_void _fl_prof_log(
    const fl_char *log_header,
    const fl_char *filename,
    const fl_char *input_label,
    fl_size input_size,
    fl_double *measures,
    fl_size measures_count);