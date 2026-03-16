#pragma once

#include "shared/common.h"

struct _fl_operator_info
{
    fl_int order;
};
typedef struct _fl_operator_info fl_operator_info;

FL_HEADER_FN fl_void fl_operator_info_init(fl_operator_info **info, fl_bool grouped)
{
    fl_operator_info *i = fl_mem_alloc(sizeof(fl_operator_info));

    i->order = grouped ? FL_INT_MAX : 0;

    *info = i;
}

FL_HEADER_FN fl_void fl_operator_info_free(fl_operator_info *info)
{
    fl_mem_free(info);
}