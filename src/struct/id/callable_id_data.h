#pragma once

// ---- DEPENDENCIES ----

#include "struct/param_arr.h"
#include "struct/arg_vec.h"
#include "struct/byte_vec.h"

// ---- TYPE DEFINITIONS ----

typedef fl_unknown *(*fl_c_definition_ptr)(fl_arg_vec args);

struct _fl_callable_id_data
{
    fl_bool c_defined;
    fl_bool constant;
    fl_data_type return_type;
    fl_param_arr params;
    union
    {
        fl_c_definition_ptr pointer;
        fl_byte_vec body;
    };
};
/**
 * @brief Callable id value;
 * contains information about callable.
 */
typedef struct _fl_callable_id_data fl_callable_id_data;

FL_HEADER_FN fl_void fl_callable_id_data_init(
    fl_callable_id_data **data,
    fl_bool c_defined,
    fl_bool constant,
    fl_data_type return_type,
    fl_param_arr params)
{
    fl_callable_id_data *ptr = fl_mem_alloc(sizeof(fl_callable_id_data));
    ptr->c_defined = c_defined;
    ptr->return_type = return_type;
    ptr->constant = constant;
    ptr->params = params;
    fl_mem_zero(&ptr->body, sizeof(fl_byte_vec));

    *data = ptr;
}

FL_HEADER_FN fl_void fl_callable_id_data_free(fl_callable_id_data **data)
{
    fl_callable_id_data *ptr = *data;
    fl_param_arr_free(&ptr->params);
    if (ptr->c_defined)
        fl_byte_vec_free(&ptr->body);
    fl_mem_free(ptr);

    *data = FL_NULL;
}