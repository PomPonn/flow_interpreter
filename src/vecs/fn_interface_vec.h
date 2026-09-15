/**
 * ---------- fn_intf_vec.h --------------------
 *
 * Declares function intf vector logic.
 *
 * -------------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/fn/fn_interface.h"

// ------------- MACROS -----------------

#define FL_VEC_NAME fl_fn_intf_vec
#define FL_VEC_TYPE fl_fn_intf
#define FL_VEC_CAP 2
#define FL_VEC_EXCLUDE_FIND
#define FL_VEC_DESTROY(elem) fl_fn_intf_free(&elem)
#include "templates/vector.h"
