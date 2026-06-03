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

// ---------- DECLARATIONS --------------

fl_long fl_time_get_unix();
fl_long fl_time_get_ticks();
fl_double fl_time_ticks_diff(fl_long start, fl_long end);