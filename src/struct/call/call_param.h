#pragma once

#include "shared/common.h"
#include "lang/data_type.h"

struct _fl_call_param
{
    fl_data_type_mix type;
};
typedef struct _fl_call_param fl_call_param;

FL_HEADER_FN fl_void fl_call_param_init(fl_call_param *param, fl_data_type_mix type_mix)
{
    param->type = type_mix;
}

FL_HEADER_FN fl_void fl_call_param_free(fl_call_param *param)
{
    assert(FL_FALSE);
}
