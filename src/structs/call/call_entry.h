#pragma once

#include "shared/common.h"
#include "lang/types.h"
#include "structs/id/id_entry.h"

#define _FL_EXTEND_CALL_ENTRY \
    _FL_EXTEND_ID_ENTRY;      \
    fl_data_type return_type; \
    fl_bool compile_time

// abstract struct (used only as an interface)
struct _fl_call_entry
{
    _FL_EXTEND_CALL_ENTRY;
};
typedef struct _fl_fn_entry fl_fn_entry;