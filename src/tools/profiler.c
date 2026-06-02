/**
 * ---------- profiler.c ----------------
 *
 * Implements profiler logic.
 *
 * --------------------------------------
 */

#if _INCLUDE_PROFILER

#include "profiler.h"

// ---------- DEPENDENCIES --------------

#include "tools/file.h"

#include <stdio.h>

// ------------- MACROS -----------------

#define FLOAT_PRECISION "3"
// 10 KB
#define BUFFER_SIZE 1024 * 10

// ------------- GLOBALS ----------------

fl_char buffer[BUFFER_SIZE];

// ---------- DEFINITIONS ---------------

fl_void _fl_prof_log(
    const fl_char *what,
    const fl_char *filename,
    const fl_char *input_unit,
    fl_size input_size,
    fl_double *measures,
    fl_size measures_count)
{
    fl_double sum = 0;
    fl_double max = 0, min = INT32_MAX;
    fl_double med = measures[(fl_size)(measures_count / 2)];

    for (fl_size i = 0; i < measures_count; i++)
    {
        if (measures[i] < min)
            min = measures[i];
        if (measures[i] > max)
            max = measures[i];
        sum += measures[i];
    }

    sprintf(
        buffer,
        "[PROFILER LOG ENTRY]\n"
        "Timestamp: %s"
        "Expression: '%s'\n"
        "Compiler: '%s'\n"
#ifdef NDEBUG
        "Build: Release\n"
#else
        "Build: Debug\n"
#endif
        "Samples: %u calls\n"
        "Input: %u %s\n"
        "Average: %." FLOAT_PRECISION "f microseconds\n"
        "Median: %." FLOAT_PRECISION "f microseconds\n"
        "Minimum: %." FLOAT_PRECISION "f microseconds\n"
        "Maximum: %." FLOAT_PRECISION "f microseconds\n"
        "Total: %." FLOAT_PRECISION "f miliseconds (%." FLOAT_PRECISION "f microseconds)\n\n",
        fl_time_date_str(), what, _C_COMPILER_PATH,
        measures_count, input_size, input_unit,
        sum / measures_count, med, min, max, sum * 1e-3, sum);

    fl_file_append_str(filename, buffer);
}

#endif
