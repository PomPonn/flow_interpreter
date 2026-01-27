#if _INCLUDE_PROFILER

#include "tools/profiler.h"

#include "tools/file.h"

#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 512

fl_char buffer[BUFFER_SIZE];

fl_void _fl_prof_log(
    const char *log_header, const char *filename,
    const char *input_label, fl_size input_size,
    fl_double *measures, fl_size measures_count)
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

    time_t raw_time;

    time(&raw_time);

    char *time_str = asctime(localtime(&raw_time));

    sprintf(
        buffer,
        "[PROFILER LOG ENTRY]\n"
        "Time: %s"
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
        time_str, log_header, measures_count, input_size, input_label,
        sum / measures_count, min, max, sum * 1e-3, sum);

    fl_file_append(filename, buffer);
}

#endif