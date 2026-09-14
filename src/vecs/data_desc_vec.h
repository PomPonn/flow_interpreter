/**
 * ---------- data_desc_vec.h --------------------
 *
 * Declares data descriptor vector logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/data_desc.h"
#include "flow/id/id_entry.h"
#include "vecs/value_vec.h"

// ------------- MACROS -----------------

#define FL_VEC_NAME fl_data_desc_vec
#define FL_VEC_TYPE fl_data_desc
#define FL_VEC_CAP 4
#define FL_VEC_COMPARE(a, b) fl_data_desc_equal(a, b)
#include "templates/vector.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void fl_data_desc_vec_from_values(fl_data_desc_vec *vec, const fl_value_vec *vals);
FL_HEADER_FN fl_bool fl_data_desc_vec_equal(const fl_data_desc_vec *vec, const fl_data_desc_vec *other);
FL_HEADER_FN fl_bool fl_data_desc_vec_is_every_const(const fl_data_desc_vec *vec);
FL_HEADER_FN fl_bool fl_data_desc_vec_suit_args(const fl_data_desc_vec *vec, fl_value_vec args);

FL_HEADER_FN fl_bool fl_data_desc_vec_match_params(
    const fl_data_desc_vec *vec,
    const fl_data_desc_vec *params);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_void fl_data_desc_vec_from_values(fl_data_desc_vec *vec, const fl_value_vec *vals)
{
    fl_data_desc_vec_init_c(vec, vals->size);

    for (fl_size i = 0; i < vals->size; i++)
        fl_data_desc_vec_push(vec, vals->data[i].desc);
}

FL_HEADER_FN fl_bool fl_data_desc_vec_equal(const fl_data_desc_vec *vec, const fl_data_desc_vec *other)
{
    if (vec->size != other->size)
        return FL_FALSE;

    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_data_desc_equal(vec->data[i], other->data[i]))
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_bool fl_data_desc_vec_is_every_const(const fl_data_desc_vec *vec)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_bitfield_is_set(vec->data[i].traits, FL_DATA_TRAITS_CONST))
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_bool fl_data_desc_vec_match_params(
    const fl_data_desc_vec *vec,
    const fl_data_desc_vec *params)
{
    if (vec->size != params->size)
        return FL_FALSE;

    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_data_desc_match_other(vec->data[i], params->data[i]))
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_bool fl_data_desc_vec_suit_args(const fl_data_desc_vec *vec, fl_value_vec args)
{
    if (vec->size != args.size)
        return FL_FALSE;

    for (fl_size i = 0; i < vec->size; i++)
    {
        fl_value arg = args.data[i];
        if (fl_value_is_desc_by(arg, vec->data[i]))
            continue;

        if (arg.desc.type == FL_DATA_TYPE_REF)
        {
            arg = *(fl_value *)arg.data;
            if (!fl_value_is_desc_by(arg, vec->data[i]))
                return FL_FALSE;

            args.data[i] = arg;
        }
    }

    return FL_TRUE;
}