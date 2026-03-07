#pragma once

#include "shared/common.h"
#include "lang/id_entry.h"

enum _fl_data_type
{
    FL_DATA_TYPE_NULL,
    FL_DATA_TYPE_INT,
    FL_DATA_TYPE_BOOL,
    FL_DATA_TYPE_FLOAT,
    FL_DATA_TYPE_ARRAY,
    FL_DATA_TYPE_OBJECT,
};
typedef fl_byte fl_data_type;

struct _fl_func_param
{
    fl_data_type type;
    fl_unknown *data;
};
typedef struct _fl_func_param fl_func_param;

fl_void fl_func_param_init(fl_func_param *param, fl_data_type data_type);
fl_void fl_func_param_free(fl_func_param *param);

// fl_func_param_list array
#define FL_ARRAY_NAME fl_func_param_list
#define FL_ARRAY_TYPE fl_func_param
#define FL_ARRAY_PRIMITIVE FL_FALSE
#include "generics/array.h"

typedef fl_unknown *(*fl_func_ptr)(fl_func_param_list *);

// extends fl_id_entry from 'lang/id_entry.h'
struct _fl_func_entry
{
    const fl_char *name;
    fl_id_value_type type;
    fl_func_ptr pointer;
    fl_data_type return_type;
    fl_func_param_list params;
};
typedef struct _fl_func_entry fl_func_entry;

// fl_func_table vector
#define FL_VECTOR_NAME fl_func_table
#define FL_VECTOR_TYPE fl_func_entry
#define FL_VECTOR_CAP 16
#define FL_VECTOR_STRUCTS FL_TRUE
#include "generics/vector.h"

fl_void fl_func_entry_init(
    fl_func_entry *entry,
    const fl_char *name,
    fl_func_ptr ptr,
    fl_data_type return_type,
    fl_func_param_list params);

fl_void fl_func_entry_free(fl_func_entry *entry);