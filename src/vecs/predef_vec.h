#pragma once

#include "lang/predef.h"
#include "vecs/const_cstr_vec.h"

#define FL_VEC_NAME fl_predef_vec
#define FL_VEC_TYPE fl_predef
#define FL_VEC_CAP 8
#define FL_VEC_DESTROY(elem) fl_predef_free(&elem)
#define FL_VEC_EXCLUDE_FIND
#include "templates/vector.h"

FL_HEADER_FN fl_const_cstr_vec fl_predef_vec_escapes_view(const fl_predef_vec *vec)
{
    fl_const_cstr_vec aliases;
    fl_const_cstr_vec_initc(&aliases, vec->size);

    for (fl_size i = 0; i < vec->size; i++)
        if (vec->data[i].escaped == FL_TRUE)
            fl_const_cstr_vec_push(&aliases, vec->data[i].alias);

    return aliases;
}

FL_HEADER_FN fl_uint fl_predef_vec_find(const fl_predef_vec *vec, const fl_char *alias)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (fl_str_equal(vec->data[i].alias, alias))
            return (fl_uint)i;

    return FL_FIND_FAIL;
}