#pragma once

#include "shared/common.h"
#include "context/context.h"

enum _fl_lib_type
{
    FL_LIB_TYPE_NULL,
    FL_LIB_TYPE_INT,
    FL_LIB_TYPE_BOOL,
    FL_LIB_TYPE_FLOAT,
    FL_LIB_TYPE_ARRAY,
    FL_LIB_TYPE_OBJECT,
};
typedef fl_byte fl_lib_type;

struct _fl_lib_param
{
    fl_lib_type type;
    fl_void *data;
};
typedef struct _fl_lib_param fl_lib_param;

fl_void fl_lib_param_init(fl_lib_param **param, fl_lib_type data_type);
fl_void fl_lib_param_free(fl_lib_param *param);

#define FL_ARRAY_NAME fl_lib_params
#define FL_ARRAY_TYPE fl_lib_param
#define FL_ARRAY_PRIMITIVE FL_FALSE
#include "generics/array.h"

typedef fl_void *(*fl_lib_func_ptr)(fl_lib_params *);

struct _fl_lib_func_entry
{
    const fl_char *name;
    const fl_lib_type return_type;
    const fl_lib_func_ptr pointer;
};
typedef struct _fl_lib_func_entry fl_lib_func_entry;

fl_void fl_lib_register_func(fl_context *context, fl_lib_func_entry *func);