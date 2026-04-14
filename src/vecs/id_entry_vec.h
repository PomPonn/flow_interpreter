/**
 * ---------- id_entry.h ----------
 *
 * Declares flow id entry vector logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "flow/id/id_entry.h"
#include "shared/str.h"

#define FL_VEC_NAME fl_id_entry_vec
#define FL_VEC_TYPE fl_id_entry *
#define FL_VEC_CAP 16
#define FL_VEC_DESTROY(elem) fl_id_entry_free(&elem)
#define FL_VEC_EXCLUDE_FIND
#include "templates/vector.h"

FL_HEADER_FN fl_int fl_id_entry_vec_find(const fl_id_entry_vec *vec, const fl_char *id_name)
{
    for (fl_int i = 0; i < vec->size; i++)
        if (fl_str_equal(vec->data[i]->name, id_name))
            return i;

    return FL_FIND_FAIL;
}