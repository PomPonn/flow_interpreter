#pragma once

#include "shared/common.h"
#include "struct/tok/token_array.h"
#include "ast/ast_tree.h"

fl_void fl_prt_print_ast_node(fl_ast_node *root, fl_ushort nest_level);

fl_void fl_prt_print_ast_tree(fl_ast_tree *tree);

fl_void fl_prt_print_token_entries(fl_token_arr *arr);

fl_void fl_prt_print_tokens_joined(fl_token_arr *arr);