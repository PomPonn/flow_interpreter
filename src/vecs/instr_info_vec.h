/**
 * ---------- instr_info_vec.h --------------------
 *
 * Declares instruction info vector logic.
 *
 * -------------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/instr/instr_info.h"
#include "vecs/const_cstr_vec.h"

// ------------- MACROS -----------------

#define FL_VEC_NAME fl_instr_info_vec
#define FL_VEC_TYPE fl_instr_info
#define FL_VEC_CAP 8
#define FL_VEC_EXCLUDE_FIND
#include "templates/vector.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_const_cstr_vec fl_instr_info_vec_to_escapes(const fl_instr_info_vec *vec);
FL_HEADER_FN fl_instr_info *fl_instr_info_vec_find(const fl_instr_info_vec *vec, const fl_char *name);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_const_cstr_vec fl_instr_info_vec_to_escapes(const fl_instr_info_vec *vec)
{
    fl_const_cstr_vec escapes;
    fl_const_cstr_vec_init_c(&escapes, vec->size);

    for (fl_size i = 0; i < vec->size; i++)
    {
        fl_instr_info curr = vec->data[i];
        if (curr.escape && (fl_const_cstr_vec_find(&escapes, curr.name) == FL_FIND_FAIL))
            fl_const_cstr_vec_push(&escapes, curr.name);
    }

    return escapes;
}

FL_HEADER_FN fl_instr_info *fl_instr_info_vec_find(const fl_instr_info_vec *vec, const fl_char *name)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (fl_str_equal(vec->data[i].name, name))
            return &vec->data[i];

    return FL_NULL;
}
