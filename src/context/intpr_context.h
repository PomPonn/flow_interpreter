/**
 * ---------- intpr_context.h --------------------
 *
 * Declares interpreter context logic.
 *
 * -----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "context/base_context.h"
#include "vecs/op_vec.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_intpr_context
{
    fl_base_context *base;
    fl_op_vec op_stack;
};
/**
 * @brief Describes interpreter context.
 */
typedef struct _fl_intpr_context fl_intpr_context;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void fl_intpr_context_init(fl_intpr_context *cnt, fl_base_context *base);
FL_HEADER_FN fl_void fl_intpr_context_free(fl_intpr_context *cnt);

// ---------- DEFINITIONS --------------

FL_HEADER_FN fl_void fl_intpr_context_free(fl_intpr_context *cnt)
{
    fl_op_vec_free(&cnt->op_stack);
}

FL_HEADER_FN fl_void fl_intpr_context_init(fl_intpr_context *cnt, fl_base_context *base)
{
    cnt->base = base;
    fl_op_vec_init(&cnt->op_stack);
}