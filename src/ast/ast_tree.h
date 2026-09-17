/**
 * ---------- ast_tree.h --------------------------
 *
 * Declares general Abstract Syntax Tree logic.
 *
 * ------------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_token_arr);
FL_FORWARD_STRUCT(fl_ast_node);

// ---------- DECLARATIONS --------------

/**
 * @brief Generate Abstract Syntax Tree from specified token array.
 * @param arr Array of tokens from which to generate the tree.
 * @throw `FL_ERR_AST`
 * @return AST root block node.
 */
fl_ast_node *fl_ast_tree_gen(fl_token_arr *arr);
