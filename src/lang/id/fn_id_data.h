#pragma once

// ---- DEPENDENCIES ----

#include "vecs/fn_variant_vec.h"

// ---- TYPE DEFINITIONS ----

struct _fl_fn_id_data
{
    fl_fn_variant_vec variants;
};
/**
 * @brief Function id value;
 * contains information about the function.
 */
typedef struct _fl_fn_id_data fl_fn_id_data;

FL_HEADER_FN fl_void fl_fn_id_data_overload(
    fl_fn_id_data *data,
    fl_bool c_defined,
    fl_bool constant,
    fl_data_type return_type,
    fl_value_desc_vec params)
{
    fl_fn_variant init_variant;
    fl_fn_variant_init(&init_variant, c_defined, constant, return_type, params);
    fl_fn_variant_vec_push(&data->variants, init_variant);
}

FL_HEADER_FN fl_void fl_fn_id_data_init(
    fl_fn_id_data **data,
    fl_bool c_defined,
    fl_bool constant,
    fl_data_type return_type,
    fl_value_desc_vec params)
{
    fl_fn_id_data *ptr = fl_mem_alloc(sizeof(fl_fn_id_data));

    fl_fn_variant_vec_init(&ptr->variants, 0);
    fl_fn_id_data_overload(ptr, c_defined, constant, return_type, params);

    *data = ptr;
}

FL_HEADER_FN fl_void fl_fn_id_data_free(fl_fn_id_data **data)
{
    fl_fn_variant_vec_free(&(*data)->variants);

    *data = FL_NULL;
}