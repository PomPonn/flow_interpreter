#pragma once

#include "shared/common.h"

typedef fl_long fl_timestamp;

fl_timestamp fl_time_get();

fl_double fl_time_diff(fl_timestamp start, fl_timestamp end);