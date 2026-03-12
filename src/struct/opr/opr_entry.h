#pragma once

#include "struct/call/call_entry.h"
#include "struct/call/call_param.h"

typedef fl_unknown *(*fl_opr_ptr)(fl_call_param, fl_call_param);

struct _fl_opr_entry
{
    _FL_EXTEND_CALL_ENTRY;
    fl_opr_ptr pointer;
    fl_call_param left;
    fl_call_param right;
};
typedef struct _fl_opr_entry fl_opr_entry;

fl_void fl_opr_entry_init(
    fl_opr_entry **entry,
    const fl_char *name,
    fl_bool compile_time,
    fl_opr_ptr ptr,
    fl_data_type return_type,
    fl_call_param left,
    fl_call_param right);

fl_void fl_opr_entry_free(fl_opr_entry *entry);