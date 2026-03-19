#pragma once

#include "shared/common.h"

#define FL_EXTEND_ID_ENTRY \
    fl_id_entry_type type; \
    const fl_char *name;   \
    fl_bool constant

enum _fl_id_entry_type
{
    FL_ID_ENTRY_TYPE_C_CALLABLE,
    FL_ID_ENTRY_TYPE_FLOW_CALLABLE,
    FL_ID_ENTRY_TYPE_OBJ,
};
typedef fl_byte fl_id_entry_type;

// abstract struct (used as an interface)
struct _fl_id_entry
{
    FL_EXTEND_ID_ENTRY;
};
typedef struct _fl_id_entry fl_id_entry;

FL_HEADER_FN fl_void fl_id_entry_fill(
    fl_id_entry *entry,
    const fl_char *name,
    fl_bool constant)
{
    entry->name = name;
    entry->constant = constant;
}

fl_void fl_id_entry_free(fl_id_entry **entry);

FL_HEADER_FN fl_bool fl_id_entry_callable(const fl_id_entry *entry)
{
    return entry->type == FL_ID_ENTRY_TYPE_C_CALLABLE || entry->type == FL_ID_ENTRY_TYPE_FLOW_CALLABLE;
}