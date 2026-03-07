#include "time.h"

#include <time.h>

fl_char *fl_time_date_str()
{
    time_t raw_time = time(FL_NULL);
    return asctime(localtime(&raw_time));
}