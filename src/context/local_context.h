/**
 * ---------- local_context.h --------------
 *
 * Declares local context logic.
 *
 * -----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "context/base_context.h"

// ------------- MACROS -----------------

// ...

// ---------- TYPE DEFINITIONS ----------

struct _fl_local_context
{
    fl_base_context *base;
    fl_scope *scope;
};
/**
 * @brief Describes generic local context. Used as an intf.
 */
typedef struct _fl_local_context fl_local_context;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void fl_local_context_deepen(fl_local_context *cnt);
FL_HEADER_FN fl_void fl_local_context_shallow(fl_local_context *cnt);

// ---------- DEFINITIONS --------------

FL_HEADER_FN fl_void fl_local_context_deepen(fl_local_context *cnt)
{
    fl_scope *scope;
    fl_scope_init(&scope, cnt->scope);
    cnt->scope = scope;
}

FL_HEADER_FN fl_void fl_local_context_shallow(fl_local_context *cnt)
{
    cnt->scope = cnt->scope->outer;
}