/**
 * ---------- ast_node.c -------------------------
 *
 * Implements Abstract Syntax Tree node logic.
 *
 * -----------------------------------------------
 */

#include "ast_node.h"

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "flow/id/id_chain.h"
#include "flow/value.h"

// ---------- DEFINITIONS ---------------

fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent)
{
    fl_ast_node *node = parent->child;

    if (!node)
        return FL_NULL;

    // Find last child
    while (node->next)
        node = node->next;

    return node;
}

fl_void fl_ast_node_append_child(fl_ast_node *parent, fl_ast_node *node)
{
    node->parent = parent;

    if (!parent->child)
    {
        parent->child = node;
        return;
    }

    fl_ast_node *current = parent->child;

    // Find last child
    while (current->next)
        current = current->next;

    current->next = node;

    // Set parent for next nodes, if any
    current = node->next;
    while (current)
    {
        current->parent = parent;
        current = current->next;
    }
}

fl_void fl_ast_node_free(fl_ast_node **node)
{
    fl_ast_node *nd = *node;

    if (!nd)
        return;

    // Free children recursively
    fl_ast_node *child = nd->child;
    while (child)
    {
        fl_ast_node *next_child = child->next;
        fl_ast_node_free(&child);
        child = next_child;
    }

    // Free current node and it's data
    switch (nd->type)
    {
    case FL_AST_NODE_TYPE_LITERAL:
        fl_value_free(nd->data);
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
        fl_id_chain_free(nd->data);
        break;
    }

    fl_mem_free(nd->data);
    fl_mem_free(nd);

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

    // Set parent
    if (parent)
        fl_ast_node_append_child(parent, n);
    else
        n->parent = FL_NULL;

    *node = n;
}

fl_ast_node *fl_ast_node_detach(fl_ast_node *node)
{
    fl_ast_node *prev = node->parent->child;

    if (prev == node)
    {
        node->parent->child = node->next;
    }
    else
    {
        while (prev->next != node)
            prev = prev->next;

        prev->next = node->next;
    }

    node->parent = FL_NULL;
    node->next = FL_NULL;

    return node;
}