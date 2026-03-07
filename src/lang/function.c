#include "function.h"

fl_void fl_func_param_init(fl_func_param *param, fl_data_type data_type)
{
    param->data = FL_NULL;
    param->type = data_type;
}

fl_void fl_func_param_free(fl_func_param *param)
{
    fl_mem_free(param->data);
}

fl_void fl_func_entry_init(
    fl_func_entry *entry,
    const fl_char *name,
    fl_func_ptr ptr,
    fl_data_type return_type,
    fl_func_param_list params)
{
    entry->name = name;
    entry->pointer = ptr;
    entry->return_type = return_type;
    entry->params = params;
}

fl_void fl_func_entry_free(fl_func_entry *entry)
{
    fl_func_param_list_free(&entry->params);
}