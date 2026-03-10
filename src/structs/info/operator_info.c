#include "operator_info.h"

#include "shared/memory.h"

fl_void fl_operator_info_init(fl_operator_info **info, fl_bool grouped)
{
    fl_operator_info *i = fl_mem_alloc(sizeof(fl_operator_info));

    i->order = grouped ? FL_UINT_MAX : 0;

    *info = i;
}

fl_void fl_operator_info_free(fl_operator_info *info)
{
    fl_mem_free(info);
}