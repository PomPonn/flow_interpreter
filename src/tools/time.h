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

// ---------- TYPE DEFINITIONS ----------

typedef fl_long fl_timestamp;

// ---------- DECLARATIONS --------------

fl_timestamp fl_time_get();
fl_char *fl_time_date_str();
fl_double fl_time_diff(fl_timestamp start, fl_timestamp end);
