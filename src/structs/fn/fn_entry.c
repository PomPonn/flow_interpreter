#include "fn_entry.h"

fl_void fl_fn_entry_init(
    fl_fn_entry **entry,
    const fl_char *name,
    fl_bool compile_time,
    fl_fn_ptr ptr,
    fl_data_type return_type,
    fl_fn_param_list params)
{
    *entry = fl_mem_alloc(sizeof(fl_fn_entry));
    (*entry)->type = FL_ID_ENTRY_TYPE_FUNC;
    (*entry)->compile_time = compile_time;
    (*entry)->name = name;
    (*entry)->pointer = ptr;
    (*entry)->return_type = return_type;
    (*entry)->params = params;
}

fl_void fl_fn_entry_free(fl_fn_entry *entry)
{
    fl_fn_param_list_free(&entry->params);
    fl_mem_free(entry);
}