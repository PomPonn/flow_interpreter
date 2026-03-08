#pragma once

#include "shared/common.h"
#include "lang/common.h"
#include "structs/id/id_entry.h"

struct _fl_fn_param
{
    fl_data_type type;
    fl_unknown *data;
};
typedef struct _fl_fn_param fl_fn_param;

fl_void fl_fn_param_init(fl_fn_param *param, fl_data_type data_type);
fl_void fl_fn_param_free(fl_fn_param *param);