#pragma once

#include "tools/lib.h"

#define FL_VEC_NAME fl_lib_handle_vec
#define FL_VEC_TYPE fl_lib_handle
#define FL_VEC_CAP 4
#define FL_VEC_DESTROY(elem) fl_lib_free(&elem)
#include "templates/vector.h"