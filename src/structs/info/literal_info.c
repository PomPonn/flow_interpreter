#include "literal_info.h"

#include "shared/memory.h"

fl_void fl_literal_info_free(fl_literal_info *info)
{
    fl_mem_free(info);
}

fl_void fl_literal_info_init(fl_literal_info **info, fl_literal_type type, fl_unknown *value)
{
    fl_literal_info *i = fl_mem_alloc(sizeof(fl_literal_info));

    i->literal_type = type;
    i->data = value;

    *info = i;
}