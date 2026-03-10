#pragma once

#include "shared/common.h"

#include "shared/memory.h"

struct _fl_operator_info
{
    fl_uint order;
};
typedef struct _fl_operator_info fl_operator_info;

fl_void fl_operator_info_init(fl_operator_info **info, fl_uint order);
fl_void fl_operator_info_free(fl_operator_info *info);