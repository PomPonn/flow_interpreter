#pragma once

#include "shared/common.h"
#include "shared/token.h"

enum _fl_ast_node_type
{
    FL_AST_NODE_TYPE_UNKNOWN,
    FL_AST_NODE_TYPE_ROOT,
    FL_AST_NODE_TYPE_TYPE,
    FL_AST_NODE_TYPE_FUNCTION,
    FL_AST_NODE_TYPE_FUNC_HEADER,
    FL_AST_NODE_TYPE_BLOCK,
    FL_AST_NODE_TYPE_IMPORT,
    FL_AST_NODE_TYPE_STATEMENT,
    FL_AST_NODE_TYPE_EXPRESSION,
    FL_AST_NODE_TYPE_LITERAL,
    FL_AST_NODE_TYPE_IDENTIFIER,
    FL_AST_NODE_TYPE_ID_CHAIN,
    FL_AST_NODE_TYPE_OPERATOR,
    // --- unused
    FL_AST_NODE_TYPE_GROUP,
    FL_AST_NODE_TYPE_CALL,
    FL_AST_NODE_TYPE_DELIMITER,
    FL_AST_NODE_TYPE_FLAG,
};
typedef fl_ubyte fl_ast_node_type;

struct _fl_ast_node
{
    struct _fl_ast_node *parent;
    struct _fl_ast_node *children;
    struct _fl_ast_node *next;
    fl_ast_node_type type;
    fl_unknown *data;
};
typedef struct _fl_ast_node fl_ast_node;

struct _fl_ast_type_node_info
{
    fl_uint dimensions;
};
typedef struct _fl_ast_type_node_info fl_ast_type_node_info;

fl_bool fl_ast_gen(fl_ast_node **root, fl_token *tokens, fl_size tokens_num);
fl_void fl_ast_free(fl_ast_node *node);
fl_void fl_ast_type_node_info_init(fl_ast_type_node_info **info, fl_uint dimensions);
fl_void fl_ast_type_node_info_free(fl_ast_type_node_info *info);

fl_void fl_ast_node_init(fl_ast_node **node, fl_ast_node_type type, fl_unknown *data, fl_ast_node *parent);
fl_void fl_ast_node_add_child(fl_ast_node *parent, fl_ast_node *node);

fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent);