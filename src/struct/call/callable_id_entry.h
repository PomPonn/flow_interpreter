#pragma once

#include "shared/common.h"
#include "struct/id/id_entry.h"
#include "struct/call/call_param_vec.h"

typedef fl_unknown *(*fl_fn_ptr)(fl_call_param_vec *);

struct _fl_callable_id_entry
{
    struct _fl_id_entry;
    fl_bool compile_time;
    fl_fn_ptr pointer;
    fl_data_type return_type;
    fl_call_param_vec params;
};
typedef struct _fl_callable_id_entry fl_callable_id_entry;

fl_void fl_callable_id_entry_init(
    fl_callable_id_entry **entry,
    const fl_char *name,
    fl_bool compile_time,
    fl_fn_ptr ptr,
    fl_data_type return_type,
    fl_call_param_vec params);

fl_void fl_callable_id_entry_free(fl_callable_id_entry *entry);