/**
 * ---------- base_context.h -----------
 *
 * Declares base context logic.
 *
 * -------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "vecs/lib_handle_vec.h"
#include "vecs/value_vec.h"
#include "vecs/cstr_vec.h"
#include "flow/scope.h"

// ---------- TYPE DEFINITIONS ------------

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

fl_void fl_base_context_init(fl_base_context *cnt);
fl_void fl_base_context_free(fl_base_context *cnt);
