#pragma once

#include "shared/common.h"
#include "shared/token.h"
#include "compiler/ast.h"

fl_void fl_prt_print_ast_tree(fl_ast_node *root, fl_ushort nest_level);

fl_void fl_prt_print_token_entries(fl_token *tokens, fl_size n);

fl_void fl_prt_print_tokens_joined(fl_token *tokens, fl_size n);