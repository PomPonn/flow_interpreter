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
