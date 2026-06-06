/**
 * ---------- time.h --------------------
 *
 * Implements cross-platform
 * time manipulation logic.
 *
 * --------------------------------------
 */

#include "time.h"

// ---------- DEPENDENCIES --------------

#include <time.h>

// ---------- DEFINITIONS ---------------

fl_long fl_time_get_unix()
{
    return time(FL_NULL);
}

fl_char *fl_time_to_str(fl_long unix_time)
{
    if (!unix_time)
        unix_time = time(FL_NULL);

    return asctime(localtime(&unix_time));
}