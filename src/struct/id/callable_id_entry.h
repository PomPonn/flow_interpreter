#pragma once

#include "shared/common.h"
#include "struct/id/id_entry.h"
#include "struct/call/call_param_vec.h"

typedef fl_unknown *(*fl_fn_ptr)(fl_call_param_vec *);

struct _fl_callable_id_entry
{
    FL_EXTEND_ID_ENTRY;
    fl_bool compile_time;
    fl_fn_ptr pointer;
    fl_data_type return_type;
    fl_call_param_vec params;
};
typedef struct _fl_callable_id_entry fl_callable_id_entry;

FL_HEADER_FN fl_void fl_callable_id_entry_init(
    fl_callable_id_entry **entry,
    const fl_char *name,
    fl_bool compile_time,
    fl_fn_ptr ptr,
    fl_data_type return_type,
    fl_call_param_vec params)
{
    fl_callable_id_entry *val = fl_mem_alloc(sizeof(fl_callable_id_entry));

    val->type = FL_ID_ENTRY_TYPE_CALLABLE;
    val->name = name;
    val->compile_time = compile_time;
    val->pointer = ptr;
    val->return_type = return_type;
    val->params = params;

    *entry = val;
}

FL_HEADER_FN fl_void fl_callable_id_entry_free(fl_callable_id_entry *entry)
{
    fl_call_param_vec_free(&entry->params);
    fl_mem_free(entry);
}