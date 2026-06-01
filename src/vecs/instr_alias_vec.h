#pragma once

#include "flow/instr/instr_alias.h"
#include "vecs/const_cstr_vec.h"

#define FL_VEC_NAME fl_instr_alias_vec
#define FL_VEC_TYPE fl_instr_alias
#define FL_VEC_CAP 8
#define FL_VEC_EXCLUDE_FIND
#include "templates/vector.h"

FL_HEADER_FN fl_const_cstr_vec fl_instr_alias_vec_to_escapes(const fl_instr_alias_vec *vec)
{
    fl_const_cstr_vec escapes;
    fl_const_cstr_vec_initc(&escapes, vec->size);

    for (fl_size i = 0; i < vec->size; i++)
        if (vec->data[i].escape)
            fl_const_cstr_vec_push(&escapes, vec->data[i].alias);

    return escapes;
}

FL_HEADER_FN fl_instr_alias *fl_instr_alias_vec_find(const fl_instr_alias_vec *vec, const fl_char *alias)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (fl_str_equal(vec->data[i].alias, alias))
            return &vec->data[i];

    return FL_NULL;
}
