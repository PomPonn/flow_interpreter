#include "callable_id_entry.h"

fl_void fl_callable_id_entry_init(
    fl_callable_id_entry **entry,
    const fl_char *name,
    fl_bool compile_time,
    fl_fn_ptr ptr,
    fl_data_type return_type,
    fl_call_param_vec params)
{
    *entry = fl_mem_alloc(sizeof(fl_callable_id_entry));
    (*entry)->type = FL_ID_ENTRY_TYPE_CALLABLE;
    (*entry)->name = name;
    (*entry)->compile_time = compile_time;
    (*entry)->pointer = ptr;
    (*entry)->return_type = return_type;
    (*entry)->params = params;
}

fl_void fl_callable_id_entry_free(fl_callable_id_entry *entry)
{
    fl_call_param_vec_free(&entry->params);
    fl_mem_free(entry);
}