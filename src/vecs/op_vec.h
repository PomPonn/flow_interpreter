/**
 * ---------- op_vec.h --------------------
 *
 * Declares operation vector logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/operation.h"

// ------------- MACROS -----------------

#define FL_VEC_NAME fl_op_vec
#define FL_VEC_TYPE fl_operation
#define FL_VEC_CAP 16
#define FL_VEC_EXCLUDE_FIND
#define FL_VEC_DESTROY(elem) fl_operation_free(&elem)
#define FL_VEC_NULL_VALUE fl_operation_uninitialized()
#include "templates/vector.h"
