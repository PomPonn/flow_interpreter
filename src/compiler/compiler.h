/**
 * ---------- compiler.h --------------
 *
 * Declares compiler logic.
 *
 * ------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES -----------

#include "flow/context.h"
#include "ast/ast_node.h"

// ---------- DECLARATIONS -----------

fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *node, fl_context *cnt);