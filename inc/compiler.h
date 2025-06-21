#pragma once

#include "common.h"

enum _ast_node_type {
  AST_NODE_TYPE_UNKNOWN = 0,
  AST_NODE_TYPE_ROOT,
  AST_NODE_TYPE_IMPORT,
  AST_NODE_TYPE_EXPRESSION,
  AST_NODE_TYPE_STATEMENT,
  AST_NODE_TYPE_CALL,
  AST_NODE_TYPE_LITERAL,
  AST_NODE_TYPE_OPERATOR,
  AST_NODE_TYPE_IDENTIFIER,
  AST_NODE_TYPE_DELIMITER,
};
typedef enum _ast_node_type ast_node_type;

struct _ast_node {
  struct _ast_node* parent;
  struct _ast_node* children;
  struct _ast_node* next;
  ast_node_type type;
  void* data;
};
typedef struct _ast_node ast_node;

void cmp_compile_to_intermediate(inp_token* tokens, size_t tokens_num);

ast_node* cmp_ast_gen_tree(inp_token* tokens, size_t tokens_num);

void cmp_ast_free_tree(ast_node* root);

ast_node* cmp_create_ast_node(ast_node* parent, ast_node* children,
                              ast_node* next, ast_node_type type);
void cmp_free_ast_node(ast_node* node);
