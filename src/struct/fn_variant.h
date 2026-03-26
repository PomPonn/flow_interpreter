#pragma once

// ---- DEPENDENCIES ----

#include "struct/value_desc_vec.h"
#include "struct/arg_vec.h"
#include "struct/byte_vec.h"

// ---- TYPE DEFINITIONS ----

typedef fl_unknown *(*fl_c_fn_ptr)(fl_arg_vec args);

struct _fl_fn_variant
{
    fl_bool c_defined;
    fl_bool constant;
    fl_data_type return_type;
    fl_value_desc_vec params;
    union
    {
        fl_c_fn_ptr pointer;
        fl_byte_vec body;
    };
};
/**
 * @brief Function variant;
 * contains information about function variant (overload).
 */
typedef struct _fl_fn_variant fl_fn_variant;

FL_HEADER_FN fl_void fl_fn_variant_init(
    fl_fn_variant *variant,
    fl_bool c_defined,
    fl_bool constant,
    fl_data_type return_type,
    fl_value_desc_vec params)
{
    variant->c_defined = c_defined;
    variant->constant = constant;
    variant->return_type = return_type;
    variant->params = params;
    fl_mem_zero(&variant->body, sizeof(fl_byte_vec));
}

FL_HEADER_FN fl_void fl_fn_variant_free(fl_fn_variant *variant)
{
    fl_value_desc_vec_free(&variant->params);
    if (variant->c_defined)
        fl_byte_vec_free(&variant->body);
}