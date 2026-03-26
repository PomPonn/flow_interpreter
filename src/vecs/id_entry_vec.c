#include "id_entry_vec.h"

#include "shared/str.h"
#include "lang/id/scope_id_data.h"

fl_int fl_id_entry_vec_find(const fl_id_entry_vec *vec, const fl_char *id_name)
{
    for (fl_int i = 0; i < vec->size; i++)
        if (fl_str_equal(vec->data[i]->name, id_name))
            return i;

    return FL_FIND_FAIL;
}