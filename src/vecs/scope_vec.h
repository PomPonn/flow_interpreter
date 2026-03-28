#pragma once

#include "lang/scope.h"

#define FL_VEC_NAME fl_scope_vec
#define FL_VEC_TYPE fl_scope *
#define FL_VEC_CAP 4
#define FL_VEC_DESTROY(elem) fl_scope_free(elem)
#include "templates/vector.h"