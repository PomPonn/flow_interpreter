/**
 * ---------- context.h ----------
 *
 * Declares language context logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "vecs/const_scope_vec.h"
#include "vecs/lib_handle_vec.h"
#include "vecs/value_vec.h"
#include "vecs/byte_vec.h"
#include "vecs/cstr_vec.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_context
{
    fl_lib_handle_vec lib_handles;
    fl_cstr_vec lib_names;
    fl_value_vec literals;
    fl_scope main_scope;
};
/**
 * @brief Describes language context, storing global information (?).
 */
typedef struct _fl_context fl_context;

/**
 * @brief Initializes context structure.
 * @param cnt Context ot initialize.
 */
FL_HEADER_FN fl_void fl_context_init(fl_context *cnt)
{
    fl_lib_handle_vec_init(&cnt->lib_handles);
    fl_cstr_vec_init(&cnt->lib_names);
    fl_scope_init(&cnt->main_scope, FL_NULL);
    fl_value_vec_init(&cnt->literals);
}

FL_HEADER_FN fl_void fl_context_free(fl_context *cnt)
{
    fl_lib_handle_vec_free(&cnt->lib_handles);
    fl_cstr_vec_free(&cnt->lib_names);
    fl_scope_free(&cnt->main_scope);
    fl_value_vec_free(&cnt->literals);
}

// TODO: comment when finished

/**
 * @brief
 * @param cnt
 * @param parent
 * @param entry
 * @return
 */
fl_void fl_context_register_id(fl_context *cnt, fl_scope *scope, fl_id_entry *entry);

FL_HEADER_FN fl_size fl_context_make_literal(fl_context *cnt, fl_value lit_value)
{
    fl_int ind = fl_value_vec_find_identical(&cnt->literals, lit_value);

    if (ind == FL_FIND_FAIL)
    {
        fl_value_vec_push(&cnt->literals, lit_value);
        return cnt->literals.size - 1;
    }

    return (fl_size)ind;
}