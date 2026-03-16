#pragma once

#include "shared/common.h"
#include "struct/id/id_entry.h"

struct _fl_obj_id_entry
{
    FL_EXTEND_ID_ENTRY;
    // ?...
};
typedef struct _fl_obj_id_entry fl_obj_id_entry;

FL_HEADER_FN fl_void fl_obj_id_entry_init(fl_obj_id_entry **entry, const fl_char *name)
{
    *entry = fl_mem_alloc(sizeof(fl_obj_id_entry));
    (*entry)->type = FL_ID_ENTRY_TYPE_OBJ;
    (*entry)->name = name;
}

FL_HEADER_FN fl_void fl_obj_id_entry_free(fl_obj_id_entry *entry)
{
    fl_mem_free(entry);
}