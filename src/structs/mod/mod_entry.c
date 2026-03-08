#include "mod_entry.h"

#include "shared/memory.h"

fl_void fl_mod_entry_init(fl_mod_entry **entry, const fl_char *name)
{
    *entry = fl_mem_alloc(sizeof(fl_mod_entry));
    (*entry)->type = FL_ID_ENTRY_TYPE_OBJ;
    (*entry)->name = name;
}

fl_void fl_mod_entry_free(fl_mod_entry *entry)
{
    fl_mem_free(entry);
}