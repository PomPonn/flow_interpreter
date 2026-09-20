/**
 * ---------- ast_node.h -----------------------
 *
 * Declares Abstract Syntax Tree node logic.
 *
 * ---------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "tokenizer/token_location.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_ast_node_type
{
    // Fallback node type.
    FL_AST_NODE_TYPE_UNKNOWN,

    // Code block.
    FL_AST_NODE_TYPE_BLOCK,

    // AST literal - a number, string etc.
    FL_AST_NODE_TYPE_LITERAL,

    // Chained identifiers.
    FL_AST_NODE_TYPE_ID_CHAIN,

    // Operator node, which consists of one id chain node and two argument nodes (of varying type).
    FL_AST_NODE_TYPE_OPERATOR,

    // Function call, first child is an id chain node, the rest are the arguments.
    FL_AST_NODE_TYPE_FN_CALL,

    // Array node, contains all elements as children.
    FL_AST_NODE_TYPE_ARRAY,
};

/**
 * @brief Abstract Syntax Tree node type - `FL_AST_NODE_TYPE_*`.
 */
typedef fl_byte fl_ast_node_type;

struct _fl_ast_node
{
    // Parent of the node. `FL_NULL` when has no parent.
    struct _fl_ast_node *parent;

    // First child of the node. `FL_NULL` when has no child.
    struct _fl_ast_node *child;

    // Next linked node. `FL_NULL` when current node is the last one.
    struct _fl_ast_node *next;

    // Type of the node.
    fl_ast_node_type type;

    // File location of this node.
    fl_token_location location;

    // Data associated with this node.
    fl_unknown *data;
};

/**
 * @brief Abstract Syntax Tree node, implemented in tree-like structure.
 */
typedef struct _fl_ast_node fl_ast_node;

// ---------- DECLARATIONS --------------

/**
 * @brief Initalize `fl_ast_node` structure.
 * @param [out] node Node to initialize, allocated on heap.
 * @param type Node type.
 * @param data Node data.
 * @param parent Node parent.
 * @param location Node file location.
 */
fl_void fl_ast_node_init(
    fl_ast_node **node,
    fl_ast_node_type type,
    fl_unknown *data,
    fl_ast_node *parent,
    fl_token_location location);

/**
 * @brief Free `fl_ast_node` structure.
 * @param node Node to free (the pointer is freed too).
 */
fl_void fl_ast_node_free(fl_ast_node **node);

/**
 * @brief Add child(ren) to the node at the end of children list.
 * @param parent Node to which the child should be added.
 * @param node Node(s) to add.
 */
fl_void fl_ast_node_append_child(fl_ast_node *parent, fl_ast_node *node);

/**
 * @brief Peek child of the node from the end of the children list.
 * @param parent Node from which peek last child.
 * @return Node's last child or `FL_NULL` if there are no chldren.
 */
fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent);

/**
 * @brief Detach node from it's node tree.
 * @param node Node to detach.
 * @return Detached node.
 */
fl_ast_node *fl_ast_node_detach(fl_ast_node *node);

/**
 * @brief Insert node after given one.
 * @param prev Previous node.
 * @param node Node to insert.
 */
FL_SEALED_FN fl_void fl_ast_node_insert_after(fl_ast_node *prev, fl_ast_node *node);

// ---------- DEFINITIONS ----------

FL_SEALED_FN fl_void fl_ast_node_insert_after(fl_ast_node *prev, fl_ast_node *node)
{
    fl_ast_node *temp = prev->next;
    prev->next = node;
    node->parent = prev->parent;
    node->next = temp;
}