/**
 * ---------- time.h --------------------
 *
 * Declares time manipulation logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ------------- MACROS -----------------

#define FL_TIME_NULL 0

// ---------- DECLARATIONS --------------

fl_long fl_time_get_unix();
fl_long fl_time_get_ticks();
fl_double fl_time_ticks_diff(fl_long start, fl_long end);
// don't free resulting string
fl_char *fl_time_to_str(fl_long unix_time);