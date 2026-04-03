#pragma once

#include "lang/value_desc.h"

#define FL_VEC_NAME fl_value_desc_vec
#define FL_VEC_TYPE fl_value_desc
#define FL_VEC_CAP 4
#define FL_VEC_COMPARE(a, b) fl_value_desc_equal(a, b)
#include "templates/vector.h"

FL_HEADER_FN fl_bool fl_value_desc_vec_equal(
    const fl_value_desc_vec *vec,
    const fl_value_desc_vec *other)
{
    if (vec->size != other->size)
        return FL_FALSE;

    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_value_desc_equal(vec->data[i], other->data[i]))
            return FL_FALSE;

    return FL_TRUE;
}