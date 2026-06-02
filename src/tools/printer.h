/**
 * ---------- printer.h ----------------------
 *
 * Declares console printing module logic.
 *
 * -------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_token_arr);
FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_ast_node);

// ---------- DECLARATIONS --------------

fl_void fl_prt_print_ast_node(fl_ast_node *root, fl_ushort nest_level);

fl_void fl_prt_print_token_entries(fl_token_arr *arr);

fl_void fl_prt_print_tokens_joined(fl_token_arr *arr);

fl_void fl_prt_print_byte_code(const fl_byte_vec bytes);

FL_HEADER_FN fl_void fl_prt_print_ast_tree(fl_ast_node *tree);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_void fl_prt_print_ast_tree(fl_ast_node *tree)
{
    fl_prt_print_ast_node(tree, 0);
}
