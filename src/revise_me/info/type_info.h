#pragma once

#include "shared/common.h"

struct _fl_type_info
{
    fl_uint dimensions;
};
typedef struct _fl_type_info fl_type_info;

FL_HEADER_FN fl_void fl_type_info_init(fl_type_info **info, fl_uint dimensions)
{
    fl_type_info *i = fl_mem_alloc(sizeof(fl_type_info));

    i->dimensions = dimensions;

    *info = i;
}

FL_HEADER_FN fl_void fl_type_info_free(fl_type_info **info)
{
    fl_mem_free(*info);
    *info = FL_NULL;
}