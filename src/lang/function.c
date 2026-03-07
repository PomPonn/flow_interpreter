#include "function.h"

fl_void fl_func_param_init(fl_func_param **param, fl_data_type data_type)
{
    *param = fl_mem_alloc(sizeof(fl_func_param));
    (*param)->data = FL_NULL;
    (*param)->type = data_type;
}

fl_void fl_func_param_free(fl_func_param *param)
{
    fl_mem_free(param->data);
    fl_mem_free(param);
}

fl_void fl_func_entry_init(
    fl_func_entry *entry,
    const fl_char *name,
    fl_func_ptr ptr,
    fl_data_type return_type,
    fl_func_params params)
{
    entry->name = name;
    entry->pointer = ptr;
    entry->return_type = return_type;
    entry->params = params;
}

fl_void fl_func_entry_free(fl_func_entry *entry)
{
    fl_func_params_free(&entry->params);
}