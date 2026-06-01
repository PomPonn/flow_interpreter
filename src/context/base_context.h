/**
 * ---------- base_context.h ----------
 *
 * Declares base context interface logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "vecs/lib_handle_vec.h"
#include "vecs/value_vec.h"
#include "vecs/cstr_vec.h"
#include "flow/scope.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_base_context
{
    fl_lib_handle_vec lib_handles;
    fl_cstr_vec lib_names;
    fl_value_vec literals;
    fl_scope *scope;
};
/**
 * @brief Describes base context.
 */
typedef struct _fl_base_context fl_base_context;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_void fl_base_context_init(fl_base_context *cnt);
FL_HEADER_FN fl_void fl_base_context_free(fl_base_context *cnt);

// ---------- DEFINITIONS -----------

FL_HEADER_FN fl_void fl_base_context_init(fl_base_context *cnt)
{
    fl_lib_handle_vec_init(&cnt->lib_handles);
    fl_cstr_vec_init(&cnt->lib_names);
    fl_value_vec_init(&cnt->literals);

    cnt->scope = fl_mem_alloc_t(fl_scope);
    fl_scope_init(cnt->scope, FL_NULL);
}

FL_HEADER_FN fl_void fl_base_context_free(fl_base_context *cnt)
{
    fl_lib_handle_vec_free(&cnt->lib_handles);
    fl_cstr_vec_free(&cnt->lib_names);
    fl_value_vec_free(&cnt->literals);

    fl_scope_free(cnt->scope);
    fl_mem_free(cnt->scope);
}