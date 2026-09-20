/**
 * ---------- intpr_context.h --------------
 *
 * Declares interpreter context logic.
 *
 * -----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "vecs/op_vec.h"
#include "context/local_context.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_intpr_context
{
    // Context base scope.
    fl_base_context *base;

    // Current context scope.
    fl_scope *scope;

    // Operation stack associated with this context.
    fl_op_vec op_stack;
};

/**
 * @brief Interpreter context.
 */
typedef struct _fl_intpr_context fl_intpr_context;

// ---------- DECLARATIONS --------------

/**
 * @brief Initialize interpretation context.
 * @param cnt Uninitialized context to init.
 * @param base Base context to connect to this context.
 */
FL_INLINE_FN fl_void fl_intpr_context_init(fl_intpr_context *cnt, fl_base_context *base);

/**
 * @brief Initialize interpretation context from given local context.
 * @param cnt Uninitialized context to init.
 * @param ref Local context to use.
 */
FL_INLINE_FN fl_void fl_intpr_context_from(fl_intpr_context *cnt, const fl_local_context *ref);

/**
 * @brief Free interpretation context.
 * @param cnt Context to free.
 */
FL_INLINE_FN fl_void fl_intpr_context_free(fl_intpr_context *cnt);

// ---------- DEFINITIONS --------------

FL_INLINE_FN fl_void fl_intpr_context_free(fl_intpr_context *cnt)
{
    fl_op_vec_free(&cnt->op_stack);
    cnt->base = FL_NULL;
    cnt->scope = FL_NULL;
}

FL_INLINE_FN fl_void fl_intpr_context_from(fl_intpr_context *cnt, const fl_local_context *ref)
{
    fl_intpr_context_init(cnt, ref->base);
    cnt->scope = ref->scope;
}

FL_INLINE_FN fl_void fl_intpr_context_init(fl_intpr_context *cnt, fl_base_context *base)
{
    cnt->base = base;
    cnt->scope = base->main_scope;
    fl_op_vec_init(&cnt->op_stack);
}