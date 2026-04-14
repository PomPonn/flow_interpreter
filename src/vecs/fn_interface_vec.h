#pragma once

#include "flow/fn/fn_variant.h"
#include "flow/fn/param.h"

#define FL_VEC_NAME fl_fn_interface_vec
#define FL_VEC_TYPE fl_fn_interface
#define FL_VEC_CAP 2
#define FL_VEC_EXCLUDE_FIND
#define FL_VEC_DESTROY(elem) fl_fn_interface_free(&elem)
#include "templates/vector.h"