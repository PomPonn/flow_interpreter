#pragma once

#include "struct/id/callable_id_entry.h"

typedef fl_unknown *(*fl_c_callable_ptr)(fl_unknown *);

struct _fl_c_callable_id_entry
{
    FL_EXTEND_CALLABLE_ID_ENTRY;
    fl_c_callable_ptr pointer;
};
typedef struct _fl_c_callable_id_entry fl_c_callable_id_entry;

FL_HEADER_FN fl_void fl_c_callable_id_entry_init(
    fl_c_callable_id_entry **entry,
    const fl_char *name,
    fl_bool constant,
    fl_data_type return_type,
    fl_call_param_vec params,
    fl_c_callable_ptr pointer)
{

    fl_c_callable_id_entry *val = fl_mem_alloc(sizeof(fl_c_callable_id_entry));
    fl_callable_id_entry_fill(
        (fl_callable_id_entry *)entry,
        name,
        FL_ID_ENTRY_TYPE_C_CALLABLE,
        constant,
        return_type,
        params);
    val->pointer = pointer;
}

FL_HEADER_FN fl_void fl_c_callable_id_entry_free(fl_c_callable_id_entry **entry)
{
    fl_c_callable_id_entry *ptr = *entry;
    fl_call_param_vec_free(&ptr->params);
    fl_mem_free(ptr);
    *entry = FL_NULL;
}