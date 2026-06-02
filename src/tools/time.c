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

fl_char *fl_time_date_str()
{
    time_t raw_time = time(FL_NULL);
    return asctime(localtime(&raw_time));
}
