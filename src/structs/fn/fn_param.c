#include "fn_param.h"

fl_void fl_fn_param_init(fl_fn_param *param, fl_data_type data_type)
{
    param->data = FL_NULL;
    param->type = data_type;
}

fl_void fl_fn_param_free(fl_fn_param *param)
{
    fl_mem_free(param->data);
}