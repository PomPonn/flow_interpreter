#include "compiler.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static inline void _cmp_ast_gen_import_children(ast_node* import_node,
                                                inp_token* tokens,
                                                size_t* index) {
  assert(import_node != NULL);
  assert(tokens != NULL);
  assert(index != NULL);

  // Assuming the next token is an identifier for the import
  if (tokens[*index].type == COMP_TOKEN_TYPE_IDENTIFIER) {
    ast_node* identifier_node =
        cmp_create_ast_node(import_node, NULL, NULL, AST_NODE_TYPE_IDENTIFIER);
    identifier_node->data = _strdup(tokens[*index].value);
    import_node->children = identifier_node;
    (*index)++;
  }
}

void cmp_free_ast_node(ast_node* node) {
  if (node == NULL) return;

  free(node);
}

ast_node* cmp_create_ast_node(ast_node* parent, ast_node* children,
                              ast_node* next, ast_node_type type) {
  ast_node* node = malloc(sizeof(ast_node));

  node->parent = parent;
  node->children = children;
  node->next = next;
  node->type = type;

  return node;
}

void cmp_ast_free_tree(ast_node* root) {
  if (root == NULL) return;

  // Free children recursively
  ast_node* child = root->children;
  while (child) {
    ast_node* next_child = child->next;
    cmp_ast_free_tree(child);
    child = next_child;
  }

  // Free the current node
  cmp_ast_free_tree(root);
  root = NULL;
}

static inline void _cmp_ast_gen_subtree(ast_node* parent, inp_token* tokens,
                                        size_t* index) {
  assert(parent != NULL);
  assert(tokens != NULL);
  assert(index != NULL);

  switch (tokens[*index].type) {
    case COMP_TOKEN_TYPE_COMMENT:
      break;
    case COMP_TOKEN_TYPE_KEYWORD:
      switch (tokens[*index].alias) {
        case COMP_TOKEN_ALIAS_IMPORT:
          // Handle import keyword

          if (tokens[*index + 1].type == COMP_TOKEN_TYPE_IDENTIFIER) {
            index++;

            // Create an import AST node
            ast_node* import_node =
                cmp_create_ast_node(parent, NULL, NULL, AST_NODE_TYPE_IMPORT);

            _cmp_ast_gen_import_children(import_node, tokens, index);
            assert(import_node->children != NULL);
          }
          break;
        case COMP_TOKEN_ALIAS_EXPORT:
          // Handle export keyword
          break;
      }
      break;
    case COMP_TOKEN_TYPE_IDENTIFIER:
      // Handle identifier

      break;
  }
}

ast_node* cmp_ast_gen_tree(inp_token* tokens, size_t tokens_num) {
  if (tokens == NULL || tokens_num == 0) return NULL;

  ast_node* root = cmp_create_ast_node(NULL, NULL, NULL, AST_NODE_TYPE_ROOT);
  size_t tok_index = 0;

  _cmp_ast_gen_subtree(root, tokens, &tok_index);

  return root;
}

void cmp_compile_to_intermediate(inp_token* tokens, size_t tokens_num) {}