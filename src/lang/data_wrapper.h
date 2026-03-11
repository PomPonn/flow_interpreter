#pragma once

#include "shared/common.h"
#include "lang/data_type.h"

struct _fl_data_wrapper
{
    fl_data_type type;
    fl_unknown *data;
};
typedef struct _fl_data_wrapper fl_data_wrapper;
