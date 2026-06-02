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
#include "templates/vector.h"
