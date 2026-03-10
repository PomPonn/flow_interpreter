#pragma once

#include "shared/common.h"
#include "ast/ast_node.h"
#include "tokenizer/token_array.h"

struct _fl_ast_tree
{
    fl_ast_node *_data;
    fl_ast_node *root;
};
typedef struct _fl_ast_tree fl_ast_tree;

fl_void fl_ast_tree_gen(fl_ast_tree *tree, fl_token_arr *arr);
fl_void fl_ast_tree_free(fl_ast_tree *tree);
