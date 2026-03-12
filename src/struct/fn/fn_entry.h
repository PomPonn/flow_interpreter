#pragma once

#include "shared/common.h"
#include "struct/call/call_param_list.h"
#include "struct/call/call_entry.h"

typedef fl_unknown *(*fl_fn_ptr)(fl_call_param_list *);

struct _fl_fn_entry
{
    _FL_EXTEND_CALL_ENTRY;
    fl_fn_ptr pointer;
    fl_call_param_list params;
};
typedef struct _fl_fn_entry fl_fn_entry;

fl_void fl_fn_entry_init(
    fl_fn_entry **entry,
    const fl_char *name,
    fl_bool compile_time,
    fl_fn_ptr ptr,
    fl_data_type return_type,
    fl_call_param_list params);

fl_void fl_fn_entry_free(fl_fn_entry *entry);