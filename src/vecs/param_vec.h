/**
 * ---------- param_vec.h --------------------
 *
 * Declares parameter vector logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/fn/param.h"
#include "flow/id/id_entry.h"
#include "vecs/value_vec.h"

// ------------- MACROS -----------------

#define FL_VEC_NAME fl_param_vec
#define FL_VEC_TYPE fl_param
#define FL_VEC_CAP 4
#define FL_VEC_COMPARE(a, b) fl_param_equal(a, b)
#include "templates/vector.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_bool fl_param_vec_equal(const fl_param_vec *vec, const fl_param_vec *other);
FL_HEADER_FN fl_bool fl_param_vec_is_every_const(const fl_param_vec *vec);
FL_HEADER_FN fl_bool fl_param_vec_cascade_simulate(const fl_param_vec *vec);

FL_HEADER_FN fl_bool fl_param_vec_suit_args(
    const fl_param_vec *vec,
    fl_value_vec *args);

FL_HEADER_FN fl_bool fl_param_vec_match_params(
    const fl_param_vec *vec,
    const fl_param_vec *params);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_bool fl_param_vec_equal(const fl_param_vec *vec, const fl_param_vec *other)
{
    if (vec->size != other->size)
        return FL_FALSE;

    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_param_equal(vec->data[i], other->data[i]))
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_bool fl_param_vec_is_every_const(const fl_param_vec *vec)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_bitfield_is_set(vec->data[i].traits, FL_PARAM_TRAITS_CONST))
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_bool fl_param_vec_cascade_simulate(const fl_param_vec *vec)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (fl_bitfield_is_set(vec->data[i].traits, FL_PARAM_TRAITS_SIMULATE))
            return FL_TRUE;

    return FL_FALSE;
}

FL_HEADER_FN fl_bool fl_param_vec_match_params(
    const fl_param_vec *vec,
    const fl_param_vec *params)
{
    if (vec->size != params->size)
        return FL_FALSE;

    for (fl_size i = 0; i < vec->size; i++)
        if (!fl_param_match_param(vec->data[i], params->data[i]))
            return FL_FALSE;

    return FL_TRUE;
}

FL_HEADER_FN fl_bool fl_param_vec_suit_args(
    const fl_param_vec *vec,
    fl_value_vec *args)
{
    if (vec->size != args->size)
        return FL_FALSE;

    fl_value_vec transformed;
    fl_value_vec_init_with(&transformed, args->size, args->data);

    for (fl_size i = 0; i < vec->size; i++)
    {
        fl_value arg = args->data[i];
        if (fl_param_match_arg(vec->data[i], arg))
            continue;

        if (arg.data_type == FL_DATA_TYPE_REF)
        {
            arg = ((fl_id_entry *)arg.data)->value;
            if (!fl_param_match_arg(vec->data[i], arg))
                return FL_FALSE;

            transformed.data[i] = arg;
        }
    }

    fl_value_vec_free_shallow(args);
    *args = transformed;
    return FL_TRUE;
}