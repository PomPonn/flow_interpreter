#pragma once

#include "lang/fn_variant.h"

#define FL_VEC_NAME fl_fn_variant_vec
#define FL_VEC_TYPE fl_fn_variant
#define FL_VEC_CAP 2
#define FL_VEC_DESTROY(elem) fl_fn_variant_free(&elem)
#define FL_VEC_COMPARE(a, b) fl_fn_variant_overlaps(a, b);
#include "templates/vector.h"