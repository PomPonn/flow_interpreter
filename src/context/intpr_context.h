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
    fl_base_context *base;
    fl_scope *scope;
    fl_op_vec op_stack;
};
/**
 * @brief Describes interpreter context.
 */
typedef struct _fl_intpr_context fl_intpr_context;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void fl_intpr_context_init(fl_intpr_context *cnt, fl_base_context *base);
FL_HEADER_FN fl_void fl_intpr_context_from(fl_intpr_context *cnt, const fl_local_context *ref);
FL_HEADER_FN fl_void fl_intpr_context_free(fl_intpr_context *cnt);

// ---------- DEFINITIONS --------------

FL_HEADER_FN fl_void fl_intpr_context_free(fl_intpr_context *cnt)
{
    fl_op_vec_free(&cnt->op_stack);
    cnt->base = FL_NULL;
    cnt->scope = FL_NULL;
}

FL_HEADER_FN fl_void fl_intpr_context_from(fl_intpr_context *cnt, const fl_local_context *ref)
{
    fl_intpr_context_init(cnt, ref->base);
    cnt->scope = ref->scope;
}

FL_HEADER_FN fl_void fl_intpr_context_init(fl_intpr_context *cnt, fl_base_context *base)
{
    cnt->base = base;
    cnt->scope = base->main_scope;
    fl_op_vec_init(&cnt->op_stack);
}