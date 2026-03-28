#pragma once

#include "lang/value.h"

#define FL_VEC_NAME fl_value_vec
#define FL_VEC_TYPE fl_value
#define FL_VEC_CAP 8
#define FL_VEC_COMPARE(a, b) fl_value_equal(a, b)
#include "templates/vector.h"