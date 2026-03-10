#pragma once

#include "shared/common.h"
#include "lang/types.h"

struct _fl_literal_info
{
    fl_literal_type literal_type;
    fl_unknown *data;
};
typedef struct _fl_literal_info fl_literal_info;

fl_void fl_literal_info_init(fl_literal_info **info, fl_literal_type type, fl_unknown *data);
fl_void fl_literal_info_free(fl_literal_info *info);
