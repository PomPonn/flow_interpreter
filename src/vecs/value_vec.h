#pragma once

#include "lang/value.h"

#define FL_VEC_NAME fl_value_vec
#define FL_VEC_TYPE fl_value
#define FL_VEC_CAP 8
#define FL_VEC_COMPARE(a, b) fl_value_equal(a, b)
#include "templates/vector.h"

FL_HEADER_FN fl_int fl_value_vec_find_identical(fl_value_vec *vec, fl_value elem)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (fl_value_indentical(vec->data[i], elem))
            return (fl_int)i;

    return FL_FIND_FAIL;
}