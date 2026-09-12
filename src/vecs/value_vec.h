/**
 * ---------- value_vec.h --------------------
 *
 * Declares value vector logic.
 *
 * -------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/value.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_byte_vec);

// ------------- MACROS -----------------

#define FL_VEC_NAME fl_value_vec
#define FL_VEC_TYPE fl_value
#define FL_VEC_CAP 8
#define FL_VEC_COMPARE(a, b) fl_value_equal(a, b)
#define FL_VEC_DESTROY(elem) fl_value_free(&elem)
#include "templates/vector.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_int fl_value_vec_find_identical(fl_value_vec *vec, fl_value elem);

FL_HEADER_FN fl_void fl_value_vec_to_bytes(const fl_value_vec *vec, fl_byte_vec *bytes);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_int fl_value_vec_find_identical(fl_value_vec *vec, fl_value elem)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (fl_value_indentical(vec->data[i], elem))
            return (fl_int)i;

    return FL_FIND_FAIL;
}

FL_HEADER_FN fl_void fl_value_vec_to_bytes(const fl_value_vec *vec, fl_byte_vec *bytes)
{
    for (fl_size i = 0; i < vec->size; i++)
        fl_value_to_bytes(vec->data[i], bytes);
}
