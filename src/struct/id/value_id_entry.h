#pragma once

#include "shared/common.h"
#include "struct/id/id_entry_vec.h"

struct _fl_value_id_entry
{
    FL_EXTEND_ID_ENTRY;
    fl_unknown *value;
};
typedef struct _fl_value_id_entry fl_value_id_entry;

FL_HEADER_FN fl_void fl_value_id_entry_init(
    fl_value_id_entry **entry,
    const fl_char *name,
    fl_unknown *value)
{
    fl_value_id_entry *val = fl_mem_alloc(sizeof(fl_value_id_entry));
    fl_id_entry_fill((fl_id_entry *)val, name, FL_ID_ENTRY_TYPE_SCOPE);
    val->value = value;
    *entry = val;
}

FL_HEADER_FN fl_void fl_value_id_entry_free(fl_value_id_entry **entry)
{
    fl_value_id_entry *e = *entry;
    fl_value_free(&e->value);
    fl_mem_free(e->value);
    *entry = FL_NULL;
}