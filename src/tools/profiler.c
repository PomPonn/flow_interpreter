#if _INCLUDE_PROFILER
#include "profiler.h"

#include "tools/file.h"
#include "tools/time.h"

#include <stdio.h>

#define BUFFER_SIZE 1024

fl_char buffer[BUFFER_SIZE];

fl_void _fl_prof_log(
    const fl_char *log_header,
    const fl_char *filename,
    const fl_char *input_label,
    fl_size input_size,
    fl_double *measures,
    fl_size measures_count)
{
    fl_double sum = 0;
    fl_double max = 0, min = INT32_MAX;

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
#ifdef NDEBUG
        "Build: Release\n"
#else
        "Build: Debug\n"
#endif
        "Samples: %u calls\n"
        "Input: %u %s\n"
        "Average: %.2f microseconds\n"
        "Minimum: %.2f microseconds\n"
        "Maximum: %.2f microseconds\n"
        "Total: %.2f miliseconds (%.2f microseconds)\n\n",
        fl_time_date_str(), log_header, measures_count, input_size, input_label,
        sum / measures_count, min, max, sum * 1e-3, sum);

    fl_file_append(filename, buffer);
}

#endif