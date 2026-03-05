#pragma once

#include "shared/common.h"
#include "context/context.h"

enum _fl_lib_type
{
    FL_LIB_TYPE_NULL,
    FL_LIB_TYPE_INT,
    FL_LIB_TYPE_FLOAT,
    FL_LIB_TYPE_ARRAY,
    FL_LIB_TYPE_OBJECT,
};
typedef fl_byte fl_lib_type;

struct _fl_lib_arg
{
    fl_lib_type type;
    fl_void *data;
};
typedef struct _fl_lib_arg fl_lib_arg;

struct _fl_lib_func_entry
{
    const fl_char *name;
    const fl_lib_type return_type;
    // const fl_lib_type *return_type;
};
typedef struct _fl_lib_func_entry fl_lib_func_entry;

#define FL_VECTOR_NAME lib_args
#define FL_VECTOR_TYPE fl_lib_arg
#define FL_VECTOR_STEP 4
#define FL_VECTOR_PRIMITIVE FL_FALSE
#include "generics/vector.h"

fl_void fl_lib_register_func(fl_context *context);