#pragma once

#include "shared/common.h"
#include "struct/id/id_entry_vec.h"

struct _fl_obj_id_entry
{
    FL_EXTEND_ID_ENTRY;
    fl_id_entry_vec inner_ids;
};
typedef struct _fl_obj_id_entry fl_obj_id_entry;

FL_HEADER_FN fl_void fl_obj_id_entry_init(fl_obj_id_entry **entry, const fl_char *name, fl_bool constant)
{
    fl_obj_id_entry *val = fl_mem_alloc(sizeof(fl_obj_id_entry));
    fl_id_entry_fill(val, name, FL_ID_ENTRY_TYPE_OBJ, constant);
    fl_id_entry_vec_init(&val->inner_ids, 0);
    *entry = val;
}

FL_HEADER_FN fl_void fl_obj_id_entry_free(fl_obj_id_entry **entry)
{
    fl_mem_free(*entry);
    *entry = FL_NULL;
}