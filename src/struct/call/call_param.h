#pragma once

#include "shared/common.h"
#include "lang/data_type.h"

struct _fl_call_param
{
    fl_data_type_mix type;
};
typedef struct _fl_call_param fl_call_param;

FL_HEADER_FN fl_void fl_call_param_init(fl_call_param *param, fl_data_type data_type)
{
    param->type = data_type;
}

FL_HEADER_FN fl_void fl_call_param_free(fl_call_param *param)
{
    assert(FL_FALSE);
}
