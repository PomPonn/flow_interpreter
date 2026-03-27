/**
 * ---- value.h ----
 *
 * Declares id value logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "lang/data_type.h"
#include "vecs/fn_variant_vec.h"

// ---- MACROS ----

#define FL_VALUE_EMPTY fl_value_create(FL_DATA_TYPE_NULL, FL_NULL)

// ---- TYPE DEFINITIONS ----

struct _fl_value
{
    fl_data_type data_type;
    fl_unknown *data;
};
typedef struct _fl_value fl_value;

// ---- DECLARATIONS ----

FL_HEADER_FN fl_value fl_value_create(fl_data_type type, fl_unknown *data);

fl_void fl_value_free(fl_value *value);
fl_value fl_value_from_literal(fl_data_type lit_type, fl_unknown *lit_data);

// ---- HEADER-ONLY DEFINITIONS ----

FL_HEADER_FN fl_value fl_value_create(fl_data_type type, fl_unknown *data)
{
    return (fl_value){
        .data = data,
        .data_type = type,
    };
}

FL_HEADER_FN fl_bool fl_value_fn_overload(fl_value *fn_value, fl_fn_variant new_variant)
{
    assert(fn_value->data_type == FL_DATA_TYPE_FN);

    fl_fn_variant_vec *vars = fn_value->data;

    for (fl_size i = 0; i < vars->size; i++)
        if (fl_fn_variant_overlaps(vars->data[i], new_variant))
            return FL_FALSE;

    fl_fn_variant_vec_push(vars, new_variant);
    return FL_TRUE;
}