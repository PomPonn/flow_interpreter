#pragma once

#include "shared/common.h"
#include "tokenizer/token_arr.h"
#include "vecs/byte_vec.h"
#include "ast/ast_tree.h"

fl_void fl_prt_print_ast_node(fl_ast_node *root, fl_ushort nest_level);

fl_void fl_prt_print_token_entries(fl_token_arr *arr);

fl_void fl_prt_print_tokens_joined(fl_token_arr *arr);

fl_void fl_prt_print_byte_code(const fl_byte_vec bytes);

FL_HEADER_FN fl_void fl_prt_print_ast_tree(fl_ast_node *tree)
{
    fl_prt_print_ast_node(tree, 0);
}