#pragma once

#include "shared/common.h"

struct _fl_type_info
{
    fl_uint dimensions;
};
typedef struct _fl_type_info fl_type_info;

fl_void fl_type_info_init(fl_type_info **info, fl_uint dimensions);
fl_void fl_type_info_free(fl_type_info *info);