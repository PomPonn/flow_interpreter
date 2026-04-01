#pragma once

#include "shared/str.h"

#define FL_VEC_NAME fl_const_cstr_vec
#define FL_VEC_TYPE const fl_char *
#define FL_VEC_CAP 8
#define FL_VEC_COMPARE(a, b) fl_str_equal(a, b)
#define FL_VEC_CONST
#include "templates/vector.h"