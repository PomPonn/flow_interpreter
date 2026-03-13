#include "literal_info.h"

#include "shared/str.h"
#include <stdlib.h>

fl_void fl_literal_info_free(fl_literal_info *info)
{
    fl_mem_free(info->data);
    fl_mem_free(info);
}

fl_void fl_literal_info_init(fl_literal_info **info, fl_literal_type type, const fl_unknown *data)
{
    fl_literal_info *i = fl_mem_alloc(sizeof(fl_literal_info));

    i->literal_type = type;

    // set decide how to use data with literal type
    switch (type)
    {
    case FL_LITERAL_TYPE_FLOAT:
        i->data = fl_mem_alloc(sizeof(fl_lang_type_float));
        *((fl_lang_type_float *)i->data) = atof(data);
        break;
    case FL_LITERAL_TYPE_INT:
        i->data = fl_mem_alloc(sizeof(fl_lang_type_int));
        *((fl_lang_type_int *)i->data) = atoi(data);
        break;
    case FL_LITERAL_TYPE_STRING:
        // len - 2 to substract quotes count
        const fl_size len = fl_str_len(data) - 2;
        i->data = fl_mem_alloc_arr(sizeof(fl_char), len);
        fl_mem_copy((fl_char *)data + 1, i->data, len);
        ((fl_char *)i->data)[len - 1] = '\0';
        break;
    case FL_LITERAL_TYPE_ARRAY:
    default:
        assert(FL_FALSE);
        break;
    }

    *info = i;
}