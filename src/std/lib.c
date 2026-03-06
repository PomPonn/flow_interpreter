#include "std/lib.h"

#include "shared/memory.h"

fl_void fl_lib_param_init(fl_lib_param **param, fl_lib_type data_type)
{
    *param = fl_mem_alloc(sizeof(fl_lib_param));
    (*param)->data = FL_NULL;
    (*param)->type = data_type;
}

fl_void fl_lib_param_free(fl_lib_param *param)
{
    fl_mem_free(param->data);
    fl_mem_free(param);
}

fl_void fl_lib_register_func(fl_context *context, fl_lib_func_entry *func)
{
    // context.
}