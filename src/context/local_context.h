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
 * @brief Describes generic local context.
 */
typedef struct _fl_local_context fl_local_context;

// ---------- DECLARATIONS --------------

FL_SEALED_FN fl_void fl_local_context_deepen(fl_local_context *cnt);
FL_SEALED_FN fl_void fl_local_context_shallow(fl_local_context *cnt);
FL_SEALED_FN fl_void fl_local_context_from(fl_local_context *cnt, const fl_local_context *ref);
FL_SEALED_FN fl_void fl_local_context_free_scope(fl_local_context *cnt);

// ---------- DEFINITIONS --------------

FL_SEALED_FN fl_void fl_local_context_from(fl_local_context *cnt, const fl_local_context *ref)
{
    cnt->base = ref->base;
    cnt->scope = ref->scope;
}

FL_SEALED_FN fl_void fl_local_context_deepen(fl_local_context *cnt)
{
    fl_scope *scope;
    fl_scope_init(&scope, cnt->scope);
    cnt->scope = scope;
}

FL_SEALED_FN fl_void fl_local_context_shallow(fl_local_context *cnt)
{
    fl_scope *prev = cnt->scope;
    cnt->scope = cnt->scope->outer;
    fl_scope_free(&prev);
}

FL_SEALED_FN fl_void fl_local_context_free_scope(fl_local_context *cnt)
{
    fl_scope_free(&cnt->scope);
}