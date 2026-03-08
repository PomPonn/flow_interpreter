#pragma once

#include "shared/common.h"
#include "structs/id/id_entry.h"

struct _fl_mod_entry
{
    FL_EXTEND_ID_ENTRY;
    // ?...
};
typedef struct _fl_mod_entry fl_mod_entry;

fl_void fl_mod_entry_init(fl_mod_entry **entry, const fl_char *name);

fl_void fl_mod_entry_free(fl_mod_entry *entry);