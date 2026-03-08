#pragma once

#include "shared/common.h"

#define FL_EXTEND_ID_ENTRY \
    const fl_char *name;   \
    fl_id_entry_type type

enum _fl_id_entry_type
{
    FL_ID_ENTRY_TYPE_FUNC,
    FL_ID_ENTRY_TYPE_OBJ,
};
typedef fl_byte fl_id_entry_type;

// abstract struct (used only as an interface)
struct _fl_id_entry
{
    FL_EXTEND_ID_ENTRY;
};
typedef struct _fl_id_entry fl_id_entry;

fl_void fl_id_entry_free(fl_id_entry *entry);