#pragma once

#include "struct/id/callable_id_entry.h"
#include "struct/byte_vec.h"

typedef fl_unknown *(*fl_flow_callable_ptr)(fl_unknown *);

struct _fl_flow_callable_id_entry
{
    FL_EXTEND_CALLABLE_ID_ENTRY;
    fl_byte_vec body;
};
typedef struct _fl_flow_callable_id_entry fl_flow_callable_id_entry;

FL_HEADER_FN fl_void fl_flow_callable_id_entry_init(
    fl_flow_callable_id_entry **entry,
    const fl_char *name,
    fl_bool constant,
    fl_data_type return_type,
    fl_call_param_vec params,
    fl_byte_vec body)
{
    fl_flow_callable_id_entry *val = fl_mem_alloc(sizeof(fl_flow_callable_id_entry));
    fl_callable_id_entry_fill(
        (fl_callable_id_entry *)entry,
        name,
        FL_ID_ENTRY_TYPE_FLOW_CALLABLE,
        constant,
        return_type,
        params);
    val->body = body;
}

FL_HEADER_FN fl_void fl_flow_callable_id_entry_free(fl_flow_callable_id_entry **entry)
{
    fl_flow_callable_id_entry *ptr = *entry;
    fl_call_param_vec_free(&ptr->params);
    fl_byte_vec_free(&ptr->body);
    fl_mem_free(ptr);
    *entry = FL_NULL;
}