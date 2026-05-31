#pragma once

#include "flow/fn/param.h"
#include "vecs/value_vec.h"

#define FL_VEC_NAME fl_param_vec
#define FL_VEC_TYPE fl_param
#define FL_VEC_CAP 4
#define FL_VEC_COMPARE(a, b) fl_param_equal(a, b)
#include "templates/vector.h"

FL_HEADER_FN fl_bool fl_param_vec_equal(
    const fl_param_vec *vec,
    const fl_param_vec *other)
{
    if (vec->size != other->size)
        return FL_FALSE;

    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_param_equal(vec->data[i], other->data[i]))
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_bool fl_param_vec_suits_args(
    const fl_param_vec *vec,
    const fl_value_vec *args)
{
    if (vec->size != args->size)
        return FL_FALSE;

    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_param_suits_arg(vec->data[i], args->data[i]))
            return FL_FALSE;

    return FL_TRUE;
}