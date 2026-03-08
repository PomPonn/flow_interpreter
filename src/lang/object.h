#pragma once

#include "shared/common.h"
#include "lang/id_entry.h"

// extends fl_id_entry from 'lang/id_entry.h'
struct _fl_object_entry
{
    const fl_char *name;
    fl_id_value_type type;
    // ?...
};
typedef struct _fl_object_entry fl_object_entry;

fl_void fl_object_entry_init(fl_object_entry **entry, const fl_char *name);

fl_void fl_object_entry_free(fl_object_entry *entry);