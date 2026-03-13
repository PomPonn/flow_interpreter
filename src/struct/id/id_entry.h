#pragma once

#include "shared/common.h"

enum _fl_id_entry_type
{
    FL_ID_ENTRY_TYPE_CALLABLE,
    FL_ID_ENTRY_TYPE_OBJ,
};
typedef fl_byte fl_id_entry_type;

// abstract struct (used as an interface)
struct _fl_id_entry
{
    const fl_char *name;
    fl_id_entry_type type;
};
typedef struct _fl_id_entry fl_id_entry;

fl_void fl_id_entry_free(fl_id_entry *entry);