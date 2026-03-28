#pragma once

#include "lang/fn_variant.h"
#include "lang/value_desc.h"

#define FL_VEC_NAME fl_fn_variant_vec
#define FL_VEC_TYPE fl_fn_variant
#define FL_VEC_CAP 2
#define FL_VEC_DESTROY(elem) fl_fn_variant_free(&elem)
#define FL_VEC_COMPARE(a, b) fl_fn_variant_overlaps(a, b)
#include "templates/vector.h"

FL_HEADER_FN fl_fn_variant *fl_fn_variant_vec_match(
    const fl_fn_variant_vec *vec,
    const fl_value_desc_vec params)
{
    for (fl_size i = 0; i < vec->size; i++)
    {
        const fl_value_desc_vec current = vec->data[i].params;

        if (current.size != params.size)
            continue;

        fl_bool skip = FL_FALSE;
        for (fl_size j = 0; j < current.size; j++)
            if (!fl_value_desc_equal(current.data[j], params.data[j]))
            {
                skip = FL_TRUE;
                break;
            }

        if (!skip)
            return &vec->data[i];
    }

    return FL_NULL;
}