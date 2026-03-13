#include "call_param.h"

fl_void fl_call_param_init(fl_call_param *param, fl_data_type data_type)
{
    param->type = data_type;
}

fl_void fl_call_param_free(fl_call_param *param)
{
    assert(FL_FALSE);
}