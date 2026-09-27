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
#include "context/local_context.h"
#include "flow/operation.h"
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
    fl_ast_node *current = parent->child;
    node->parent = parent;

    if (!current)
        parent->child = node;
    else
    {
        // Find last child
        while (current->next)
            current = current->next;

        current->next = node;
    }

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
    assert(prev);

    if (prev == node)
    {
        node->parent->child = node->next;
    }
    else
    {
        while (prev->next != node)
        {
            prev = prev->next;
            assert(prev);
        }

        prev->next = node->next;
    }

    node->parent = FL_NULL;
    node->next = FL_NULL;

    return node;
}

fl_data_desc fl_ast_node_get_desc(
    const fl_ast_node *node,
    const fl_local_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        // ???????????
        return fl_data_desc_create_traited(FL_DATA_TYPE_SCOPE | FL_DATA_TYPE_CODE_SEGM, FL_DATA_TRAIT_CONST);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_ast_node *id_node = node->child;
        fl_operation op = fl_operation_from_id_node(id_node, cnt->scope);
        fl_data_desc_vec descs = fl_ast_nodes_get_desc_vec(id_node->next, (fl_local_context *)cnt);
        const fl_fn_intf *fn_i = fl_operation_get_intf_by_descs(op, &descs);

        if (op.type == FL_OPERATION_UNKNOWN)
            return fl_data_desc_create(FL_DATA_TYPE_ANY);

        if (!fl_operation_is_callable(op))
            assert(FL_FALSE);

        assert(fn_i);

        // Init descriptor
        fl_data_traits traits = FL_DATA_TRAIT_NONE;

        // If we can predict result of the operation, it's inherently constant.
        if (fl_fn_intf_is_inlineable(fn_i, &descs))
            fl_bit_set(traits, FL_DATA_TRAIT_CONST);

        fl_data_desc_vec_free(&descs);

        return fl_data_desc_create_traited(fn_i->return_type, traits);
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_id_entry *id = fl_scope_find_id(cnt->scope, node->data);
        fl_data_type type = id ? id->decl_type : FL_DATA_TYPE_ANY;
        return fl_data_desc_create_traited(type, FL_DATA_TRAIT_REF);
    }
    case FL_AST_NODE_TYPE_LITERAL:
    {
        fl_value *lit_val = node->data;
        return lit_val->desc;
    }
    case FL_AST_NODE_TYPE_ARRAY:
        return fl_data_desc_create(FL_DATA_TYPE_ARRAY);
    default:
        assert(FL_FALSE);
        return fl_data_desc_create(FL_DATA_TYPE_NULL);
    }
}