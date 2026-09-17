/**
 * ---------- inliner.h -------------------------
 *
 * Declares inliner logic. Handles compilation
 * optimizing and compile time operations.
 *
 * ----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_ast_node);
FL_FORWARD_STRUCT(fl_operation);
FL_FORWARD_STRUCT(fl_cmp_context);

// ---------- DECLARATIONS --------------

/**
 * @brief Try to FL_SEALED_FN operation. Optimize/expand it.
 * @param id_node AST id node of the operation.
 * @param op Operation to FL_SEALED_FN.
 * @param cnt Context to use.
 * @return `FL_TRUE` when FL_SEALED_FNd successfully, otherwise `FL_FALSE`.
 */
fl_bool fl_inl_try_operation_inline(
    const fl_ast_node *id_node,
    const fl_operation op,
    fl_cmp_context *cnt);