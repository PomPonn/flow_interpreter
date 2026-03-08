#include "object.h"

#include "shared/memory.h"

fl_void fl_object_entry_init(fl_object_entry **entry, const fl_char *name)
{
    *entry = fl_mem_alloc(sizeof(fl_object_entry));
    (*entry)->type = FL_ID_VALUE_TYPE_OBJ;
    (*entry)->name = name;
}

fl_void fl_object_entry_free(fl_object_entry *entry)
{
    assert(FL_FALSE);
}