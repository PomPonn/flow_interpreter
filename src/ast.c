#include "ast.h"

#include <string.h>
#include <stdlib.h>

void ast_gen(ast_node *root, inp_token *tokens, size_t tokens_num)
{
    if (tokens == NULL || tokens_num == 0)
        return NULL;

    ast_node_init(&root, AST_NODE_TYPE_ROOT, NULL);

    size_t tok_index = 0;
    _cmp_ast_gen_subtree(root, tokens, &tok_index, tokens_num);
}

void ast_free(ast_node *node)
{
    if (node == NULL)
        return;

    // Free children recursively
    ast_node *child = node->children;
    while (child)
    {
        ast_node *next_child = child->next;
        cmp_ast_free_tree(child);
        child = next_child;
    }

    // Free current node
    free(node->data);
    free(node);
}

void ast_node_init(ast_node **node, ast_node_type type, void *data)
{
    ast_node *n = malloc(sizeof(ast_node));

    n->type = type;
    n->data = data;
    n->parent = NULL;
    n->children = NULL;
    n->next = NULL;

    *node = n;
}

void ast_node_add_child(ast_node *parent, ast_node *node)
{
    if (parent->children == NULL)
    {
        node->parent = parent;
        parent->children = node;
        return;
    }

    ast_node *current = parent->children;

    while (current->next)
        current = current->next;

    node->parent = parent;
    current->next = node;
}