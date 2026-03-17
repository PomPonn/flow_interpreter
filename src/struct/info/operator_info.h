#pragma once

#include "shared/common.h"

enum _fl_operator_order {
    FL_OPR_ORDER_DEFAULT = 0,
    FL_OPR_ORDER_ARITHM_LOW = 1000,
    FL_OPR_ORDER_ARITHM_MED = 2000,
    FL_OPR_ORDER_ARITHM_HIGH = 3000,
    FL_OPR_ORDER_PRIORITY = FL_INT_MAX - 2000,
    FL_OPR_ORDER_MAX = FL_INT_MAX - 1000,
};
typedef fl_int fl_operator_order;

struct _fl_operator_info
{
    fl_operator_order order;
};
typedef struct _fl_operator_info fl_operator_info;

FL_HEADER_FN fl_void fl_operator_info_init(fl_operator_info **info, fl_operator_order order)
{
    fl_operator_info *i = fl_mem_alloc(sizeof(fl_operator_info));
    i->order = order;
    *info = i;
}

FL_HEADER_FN fl_void fl_operator_info_free(fl_operator_info *info)
{
    fl_mem_free(info);
}
