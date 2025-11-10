#pragma once

#include "common.h"

enum _ast_node_type
{
    AST_NODE_TYPE_UNKNOWN = 0,
    AST_NODE_TYPE_ROOT,
    AST_NODE_TYPE_BLOCK,
    AST_NODE_TYPE_GROUP,
    AST_NODE_TYPE_IMPORT,
    AST_NODE_TYPE_EXPRESSION,
    AST_NODE_TYPE_STATEMENT,
    AST_NODE_TYPE_CALL,
    AST_NODE_TYPE_LITERAL,
    AST_NODE_TYPE_OPERATOR,
    AST_NODE_TYPE_IDENTIFIER,
    AST_NODE_TYPE_DELIMITER,
    AST_NODE_TYPE_FLAG,
};
typedef enum _ast_node_type ast_node_type;

struct _ast_node
{
    struct _ast_node *parent;
    struct _ast_node *children;
    struct _ast_node *next;
    ast_node_type type;
    void *data;
};
typedef struct _ast_node ast_node;

void ast_gen(ast_node *root, inp_token *tokens, size_t tokens_num);
void ast_free(ast_node *node);

void ast_node_init(ast_node **node, ast_node_type type, void *data);
void ast_node_add_child(ast_node *parent, ast_node *node);