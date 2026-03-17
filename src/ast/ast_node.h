/**
 * ---- ast_node.h ----
 *
 * Declares Abstract Syntax Tree node logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "struct/info/file_location_info.h"

// ---- TYPE DEFINITIONS ----

enum _fl_ast_node_type
{
    /**
     * @brief Fallback node type.
     */
    FL_AST_NODE_TYPE_UNKNOWN,

    /**
     * @brief Block which defines local scope.
     */
    FL_AST_NODE_TYPE_BLOCK,

    /**
     * @brief Constant, hard-coded literal - a number, string, array etc.
     */
    FL_AST_NODE_TYPE_LITERAL,

    /**
     * @brief Chain if identifiers, that point at some kind of value.
     */
    FL_AST_NODE_TYPE_ID_CHAIN,

    /**
     * @brief Operator node, which consists of one id chain node and two argument nodes (of varying type).
     */
    FL_AST_NODE_TYPE_OPERATOR,

    /**
     * @brief Function call, first child is an id chain node, the rest are the arguments.
     */
    FL_AST_NODE_TYPE_FN_CALL,

    // ---- UNUSED ----

    // TODO: comment here when finished

    FL_AST_NODE_TYPE_FUNCTION,
    FL_AST_NODE_TYPE_FUNC_HEADER,
};

/**
 * @brief Abstract Syntax Tree node type - `FL_AST_NODE_TYPE_*`.
 */
typedef fl_byte fl_ast_node_type;

struct _fl_ast_node
{
    /**
     * @brief Parent of the node.
     * `FL_NULL` when has no parent.
     */
    struct _fl_ast_node *parent;

    /**
     * @brief First child of the node.
     * `FL_NULL` when has no child.
     */
    struct _fl_ast_node *child;

    /**
     * @brief Next linked node.
     * `FL_NULL` when current node is the last one.
     */
    struct _fl_ast_node *next;

    /**
     * @brief Type of the node.
     */
    fl_ast_node_type type;

    /**
     * @brief File location of this node.
     */
    fl_file_location_info location;

    /**
     * @brief Data associated with this node.
     */
    fl_unknown *data;
};

/**
 * @brief Abstract Syntax Tree node, implemented in tree-like structure.
 */
typedef struct _fl_ast_node fl_ast_node;

// ---- DECLARATIONS ----

/**
 * @brief Initalizes `fl_ast_node` structure.
 * @param [out] node Node to initialize.
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
    fl_file_location_info location);

/**
 * @brief Frees `fl_ast_node` structure.
 * @param node Node to free (the pointer is freed too).
 */
fl_void fl_ast_node_free(fl_ast_node *node);

/**
 * @brief Adds child to the node at the end of children list.
 * @param parent Node to which the child should be added.
 * @param node Node to add.
 */
fl_void fl_ast_node_add_child(fl_ast_node *parent, fl_ast_node *node);

/**
 * @brief Peeks child of the node from the end of the children list.
 * @param parent Node from which peek last child.
 * @return Node's last child or `FL_NULL` if there are no chldren.
 */
fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent);