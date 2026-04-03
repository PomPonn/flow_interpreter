#pragma once

#include "lang/fn_variant.h"
#include "lang/value_desc.h"

#define FL_VEC_NAME fl_fn_variant_vec
#define FL_VEC_TYPE fl_fn_variant
#define FL_VEC_CAP 2
#define FL_VEC_COMPARE(a, b) fl_fn_variant_overlaps(a, b)
#define FL_VEC_DESTROY(elem) fl_fn_variant_free(&elem)
#include "templates/vector.h"

FL_HEADER_FN fl_fn_variant *fl_fn_variant_vec_match(
    const fl_fn_variant_vec *vec,
    const fl_value_desc_vec params)
{
    for (fl_size i = 0; i < vec->size; i++)
    {
        const fl_value_desc_vec current = vec->data[i].params;

        if (fl_value_desc_vec_equal(&current, &params))
            return &vec->data[i];
    }

    return FL_NULL;
}

FL_HEADER_FN fl_bool fl_fn_variant_vec_overload(fl_fn_variant_vec *fn_vars, fl_fn_variant new_variant)
{
    for (fl_size i = 0; i < fn_vars->size; i++)
        if (fl_fn_variant_overlaps(fn_vars->data[i], new_variant))
            return FL_FALSE;

    fl_fn_variant_vec_push(fn_vars, new_variant);
    return FL_TRUE;
}