/**
 * ---------- base_context.h -----------
 *
 * Declares base context logic.
 *
 * -------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES -------------

#include "shared/common.h"
#include "vecs/lib_handle_vec.h"
#include "vecs/value_vec.h"
#include "vecs/cstr_vec.h"
#include "flow/scope.h"

// ---------- TYPE DEFINITIONS ---------

struct _fl_base_context
{
    // Imported library handles.
    fl_lib_handle_vec lib_handles;

    // Imported library names.
    fl_cstr_vec lib_names;

    // Literals created through compilation process.
    fl_value_vec literals;

    // Entry scope of the program.
    fl_scope *main_scope;
};

/**
 * @brief Base flow context.
 */
typedef struct _fl_base_context fl_base_context;

// ---------- DECLARATIONS -------------

/**
 * @brief Init base context.
 * @param cnt Uninitialized context to init.
 */
fl_void fl_base_context_init(fl_base_context *cnt);

/**
 * @brief Free base context.
 * @param cnt Context to free.
 */
fl_void fl_base_context_free(fl_base_context *cnt);
