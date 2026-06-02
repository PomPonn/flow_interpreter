/**
 * ---------- ast_node.c ------------------------
 *
 * Implements Abstract Syntax Tree node logic.
 *
 * ----------------------------------------------
 */

#include "ast_node.h"

// ---------- DEFINITIONS ---------------

fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent)
{
    fl_ast_node *node = parent->child;

    if (!node)
        return FL_NULL;

    while (node->next)
        node = node->next;

    return node;
}

fl_void fl_ast_node_add_child(fl_ast_node *parent, fl_ast_node *node)
{
    if (!parent->child)
    {
        node->parent = parent;
        parent->child = node;
        return;
    }

    fl_ast_node *current = parent->child;

    while (current->next)
        current = current->next;

    node->parent = parent;
    current->next = node;
}

fl_void fl_ast_node_free(fl_ast_node **node)
{
    if (!*node)
        return;

    // Free children recursively
    fl_ast_node *child = (*node)->child;
    while (child)
    {
        fl_ast_node *next_child = child->next;
        fl_ast_node_free(&child);
        child = next_child;
    }

    // Free current node and it's data
    fl_mem_free((*node)->data);
    fl_mem_free(*node);

    *node = FL_NULL;
}

fl_void fl_ast_node_init(
    fl_ast_node **node,
    fl_ast_node_type type,
    fl_unknown *data,
    fl_ast_node *parent,
    fl_token_location location)
{
    fl_ast_node *n = fl_mem_alloc_t(fl_ast_node);

    n->type = type;
    n->data = data;
    n->child = FL_NULL;
    n->next = FL_NULL;
    n->location = location;

    if (parent)
        fl_ast_node_add_child(parent, n);
    else
        n->parent = FL_NULL;

    *node = n;
}
