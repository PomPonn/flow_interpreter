/**
 * ---------- compiler.h --------------
 *
 * Declares compiler logic.
 *
 * ------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS --------

FL_FORWARD_STRUCT(fl_ast_node);
FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_operation);
FL_FORWARD_STRUCT(fl_cmp_context);

// ---------- DECLARATIONS ------------

/**
 * @brief Compile AST tree to byte code.
 * @param tree AST tree to compile.
 * @return Compiled byte code.
 */
fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *tree);

/**
 * @brief Compile single AST node.
 * @param node Node to compile.
 * @param cnt Context to use while compilation.
 */
fl_void fl_cmp_compile_node(const fl_ast_node *node, fl_cmp_context *cnt);