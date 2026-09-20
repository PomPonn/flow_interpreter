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

// ---------- TYPE DEFINITIONS ----------

struct _fl_local_context
{
    // Context base scope.
    fl_base_context *base;

    // Current context scope.
    fl_scope *scope;
};

/**
 * @brief Generic local context. Can be used as an interface.
 */
typedef struct _fl_local_context fl_local_context;

// ---------- DECLARATIONS --------------

/**
 * @brief Create deeper scope and set it as current scope.
 * @param cnt Context to alter.
 */
FL_INLINE_FN fl_void fl_local_context_deepen(fl_local_context *cnt);

/**
 * @brief Free current scope and set it to the parent scope.
 * @param cnt Context to alter.
 */
FL_INLINE_FN fl_void fl_local_context_shallow(fl_local_context *cnt);

/**
 * @brief Initialize local context from the other one.
 * @param cnt Uninitialized context to init.
 * @param ref Local context to use.
 */
FL_INLINE_FN fl_void fl_local_context_from(fl_local_context *cnt, const fl_local_context *ref);

/**
 * @brief Free local context scope.
 * @param cnt Context to alter.
 */
FL_INLINE_FN fl_void fl_local_context_free_scope(fl_local_context *cnt);

// ---------- DEFINITIONS --------------

FL_INLINE_FN fl_void fl_local_context_from(fl_local_context *cnt, const fl_local_context *ref)
{
    cnt->base = ref->base;
    cnt->scope = ref->scope;
}

FL_INLINE_FN fl_void fl_local_context_deepen(fl_local_context *cnt)
{
    fl_scope *scope;
    fl_scope_init(&scope, cnt->scope);
    cnt->scope = scope;
}

FL_INLINE_FN fl_void fl_local_context_shallow(fl_local_context *cnt)
{
    fl_scope *prev = cnt->scope;
    cnt->scope = cnt->scope->outer;
    fl_scope_free(&prev);

    assert(cnt->scope);
}

FL_INLINE_FN fl_void fl_local_context_free_scope(fl_local_context *cnt)
{
    fl_scope_free(&cnt->scope);
}