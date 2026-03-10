#include "call_param.h"

fl_void fl_call_param_init(fl_call_param *param, fl_data_type data_type)
{
    param->data = FL_NULL;
    param->type = data_type;
}

fl_void fl_call_param_free(fl_call_param *param)
{
    fl_mem_free(param->data);
}