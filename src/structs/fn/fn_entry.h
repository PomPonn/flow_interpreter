#pragma once

#include "shared/common.h"
#include "lang/common.h"
#include "structs/id/id_entry.h"
#include "structs/fn/fn_param_list.h"

typedef fl_unknown *(*fl_fn_ptr)(fl_fn_param_list *);

struct _fl_fn_entry
{
    FL_EXTEND_ID_ENTRY;
    fl_fn_ptr pointer;
    fl_data_type return_type;
    fl_fn_param_list params;
};
typedef struct _fl_fn_entry fl_fn_entry;

fl_void fl_fn_entry_init(
    fl_fn_entry **entry,
    const fl_char *name,
    fl_fn_ptr ptr,
    fl_data_type return_type,
    fl_fn_param_list params);

fl_void fl_fn_entry_free(fl_fn_entry *entry);