#pragma once

#include "lang/value_desc.h"

#define FL_VEC_NAME fl_value_desc_vec
#define FL_VEC_TYPE fl_value_desc
#define FL_VEC_CAP 4
#define FL_VEC_COMPARE(a, b) fl_value_desc_equal(a, b)
#include "templates/vector.h"