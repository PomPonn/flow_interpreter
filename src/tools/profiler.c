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

#define MEDIAN(arr, count) (count % 2 == 0 ? (arr[(fl_size)(count / 2)] + arr[(fl_size)(count / 2) + 1]) / 2 : arr[(fl_size)(count / 2)])

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
    fl_double med = MEDIAN(measures, measures_count);

    for (fl_size i = 0; i < measures_count; i++)
    {
        if (measures[i] < min)
            min = measures[i];
        if (measures[i] > max)
            max = measures[i];
        sum += measures[i];
    }

    fl_double avg = sum / measures_count;

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
        "Average: %." FLOAT_PRECISION "f ms / %." FLOAT_PRECISION "f us\n"
        "Median:  %." FLOAT_PRECISION "f ms / %." FLOAT_PRECISION "f us\n"
        "Minimum:  %." FLOAT_PRECISION "f ms / %." FLOAT_PRECISION "f us\n"
        "Maximum:  %." FLOAT_PRECISION "f ms / %." FLOAT_PRECISION "f us\n"
        "Total: %." FLOAT_PRECISION "f s / %." FLOAT_PRECISION "f ms / %." FLOAT_PRECISION "f us\n\n",
        fl_time_to_str(FL_TIME_NULL), what, _C_COMPILER_PATH,
        measures_count, input_size, input_unit,
        avg * 1e-3, avg,
        med * 1e-3, med,
        min * 1e-3, min,
        max * 1e-3, max,
        sum * 1e-6, sum * 1e-3, sum);

    fl_file_append_str(filename, buffer);

    printf("{PROFILER} Saved profiler log to " _PROFILER_LOGS_PATH);
}

#endif
