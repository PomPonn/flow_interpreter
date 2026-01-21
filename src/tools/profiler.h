#pragma once

#include "tools/time.h"

#include <stdio.h>

struct _fl_prof_result
{
    double elapsed_time;
};
typedef struct _fl_prof_result fl_prof_result;

#define fl_prof_profile(segment)                                      \
    {                                                                 \
        fl_timestamp __start = fl_time_get();                         \
        segment;                                                      \
        double __elapsed_time = fl_time_diff(__start, fl_time_get()); \
        printf("Elapsed time: %fus", __elapsed_time * 1e6);           \
    }