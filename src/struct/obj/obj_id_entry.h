#pragma once

#include "shared/common.h"
#include "struct/id/id_entry.h"

struct _fl_obj_id_entry
{
    struct _fl_id_entry;
    // ?...
};
typedef struct _fl_obj_id_entry fl_obj_id_entry;

fl_void fl_obj_id_entry_init(fl_obj_id_entry **entry, const fl_char *name);

fl_void fl_obj_id_entry_free(fl_obj_id_entry *entry);