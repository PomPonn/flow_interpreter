#pragma once

#include "shared/common.h"
#include "tokenizer/token.h"

enum _fl_ast_node_type
{
    FL_AST_NODE_TYPE_UNKNOWN,
    FL_AST_NODE_TYPE_ROOT,
    FL_AST_NODE_TYPE_BLOCK,
    FL_AST_NODE_TYPE_LITERAL,
    FL_AST_NODE_TYPE_ID_CHAIN,
    FL_AST_NODE_TYPE_OPERATOR,
    FL_AST_NODE_TYPE_CALL,
    // ---- unused for now
    FL_AST_NODE_TYPE_FUNCTION,
    FL_AST_NODE_TYPE_FUNC_HEADER,
};
typedef fl_byte fl_ast_node_type;

struct _fl_ast_node
{
    struct _fl_ast_node *parent;
    struct _fl_ast_node *child;
    struct _fl_ast_node *next;
    fl_ast_node_type type;
    fl_token_location location;
    fl_unknown *data;
};
typedef struct _fl_ast_node fl_ast_node;

fl_void fl_ast_node_init(fl_ast_node **node, fl_ast_node_type type, fl_unknown *data, fl_ast_node *parent, fl_token_location location);
fl_void fl_ast_node_free(fl_ast_node *node);

fl_void fl_ast_node_add_child(fl_ast_node *parent, fl_ast_node *node);
fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent);