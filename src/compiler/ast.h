#pragma once

#include "shared/common.h"
#include "shared/token.h"
#include "shared/token_array.h"

enum _fl_ast_node_type
{
    FL_AST_NODE_TYPE_UNKNOWN,
    FL_AST_NODE_TYPE_ROOT,
    FL_AST_NODE_TYPE_BLOCK,
    FL_AST_NODE_TYPE_LITERAL,
    FL_AST_NODE_TYPE_IDENTIFIER,
    FL_AST_NODE_TYPE_ID_CHAIN,
    FL_AST_NODE_TYPE_OPERATOR,
    FL_AST_NODE_TYPE_CALL,
    // --- unused for now
    FL_AST_NODE_TYPE_FUNCTION,
    FL_AST_NODE_TYPE_FUNC_HEADER,
};
typedef fl_byte fl_ast_node_type;

enum _fl_ast_literal_type
{
    FL_AST_LITERAL_TYPE_STRING = FL_TOKEN_ALIAS_STRING,
    FL_AST_LITERAL_TYPE_NUMBER,
    FL_AST_LITERAL_TYPE_ARRAY,
};
typedef fl_byte fl_ast_literal_type;

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

struct _fl_ast_tree
{
    fl_ast_node *_data;
    fl_ast_node *root;
};
typedef struct _fl_ast_tree fl_ast_tree;

struct _fl_ast_literal_data
{
    fl_ast_literal_type literal_type;
    fl_unknown *data;
};
typedef struct _fl_ast_literal_data fl_ast_literal_data;

struct _fl_ast_type_data
{
    fl_ushort dimensions;
};
typedef struct _fl_ast_type_data fl_ast_type_data;

struct _fl_ast_operator_data
{
    fl_bool grouped;
    fl_ushort order;
};
typedef struct _fl_ast_operator_data fl_ast_operator_data;

fl_void fl_ast_tree_gen(fl_ast_tree *tree, fl_token_arr *arr);
fl_void fl_ast_tree_free(fl_ast_tree *tree);

fl_void fl_ast_type_data_init(fl_ast_type_data **info, fl_uint dimensions);
fl_void fl_ast_type_data_free(fl_ast_type_data *info);

fl_void fl_ast_literal_data_init(fl_ast_literal_data **info, fl_ast_literal_type type, fl_unknown *data);
fl_void fl_ast_literal_data_free(fl_ast_literal_data *info);

fl_void fl_ast_node_init(fl_ast_node **node, fl_ast_node_type type, fl_unknown *data, fl_ast_node *parent, fl_token_location location);
fl_void fl_ast_node_free(fl_ast_node *node);
fl_void fl_ast_node_add_child(fl_ast_node *parent, fl_ast_node *node);
fl_char *fl_ast_stringify_id_chain(fl_ast_node *node, fl_char *buffer, fl_size buffer_size);

fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent);