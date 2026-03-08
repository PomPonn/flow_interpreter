#include "obj_entry.h"

#include "shared/memory.h"

fl_void fl_obj_entry_init(fl_obj_entry **entry, const fl_char *name)
{
    *entry = fl_mem_alloc(sizeof(fl_obj_entry));
    (*entry)->type = FL_ID_ENTRY_TYPE_OBJ;
    (*entry)->name = name;
}

fl_void fl_obj_entry_free(fl_obj_entry *entry)
{
    assert(FL_FALSE);
}