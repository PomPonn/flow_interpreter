#pragma once

#include "shared/common.h"
#include "structs/id/id_entry.h"

struct _fl_obj_entry
{
    _FL_EXTEND_ID_ENTRY;
    // ?...
};
typedef struct _fl_obj_entry fl_obj_entry;

fl_void fl_obj_entry_init(fl_obj_entry **entry, const fl_char *name);

fl_void fl_obj_entry_free(fl_obj_entry *entry);