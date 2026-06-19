/**
 * ---------- fn_variant_vec_vec.h --------------------
 *
 * Declares vector of function variant vectors logic.
 *
 * ------------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "vecs/fn_variant_vec.h"

// ------------- MACROS -----------------

#define FL_VEC_NAME fl_fn_variant_vec_vec
#define FL_VEC_TYPE fl_fn_variant_vec
#define FL_VEC_DESTROY(elem) fl_fn_variant_vec_free(&elem)
#define FL_VEC_EXCLUDE_FIND
#include "templates/vector.h"

// ---------- DECLARATIONS --------------

// ---------- DEFINITIONS ---------------
