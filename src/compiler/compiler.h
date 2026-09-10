/**
 * ---------- compiler.h --------------
 *
 * Declares compiler logic.
 *
 * ------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES -----------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS -----------

FL_FORWARD_STRUCT(fl_ast_node);
FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_operation);
FL_FORWARD_STRUCT(fl_cmp_context);
FL_FORWARD_STRUCT(fl_base_context);

// ---------- DECLARATIONS -----------

fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *tree, fl_base_context *cnt);

fl_void fl_cmp_compile_node(const fl_ast_node *node, fl_cmp_context *cnt);

fl_void fl_cmp_call_to_bytes(fl_cmp_context *cnt, fl_ast_node *id_node, fl_operation op);