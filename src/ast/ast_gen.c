/**
 * ---------- ast_gen.c -------------------------------
 *
 * Implements Abstract Syntax Tree generation logic.
 *
 * ----------------------------------------------------
 */

#include "ast_gen.h"

// ---------- DEPENDENCIES -----------

#include "tokenizer/token_location.h"
#include "tokenizer/token_arr.h"
#include "vecs/instr_info_vec.h"
#include "flow/instr/g_instrs.h"
#include "flow/id/id_chain.h"
#include "ast/ast_node.h"
#include "shared/error.h"
#include "flow/value.h"
#include "shared/str.h"

#ifndef NDEBUG

#include "tools/printer.h"
#include <stdio.h>
#include <stdlib.h>

#endif

// ---------- MACROS ----------------

/**
 * @brief Shorthand for throwing syntax error.
 */
#define __throw_syntax_error(message) \
    fl_err_throw(FL_ERR_AST, message, arr->current->value, arr->current->location)

/**
 * @brief Shorthand for incrementing token pointer.
 */
#define __next_token \
    fl_token_arr_next(arr)

/**
 * @brief Shorthand for incrementing token pointer, and throwing an error if it's end of code.
 */
#define __next_token_safe \
    if (!__next_token)    \
    fl_err_throw(FL_ERR_AST, "Unexpected end of code (missing ';'?)", arr->_data[arr->_iterator].value, arr->_data[arr->_iterator].location)

/**
 * @brief Shorthand for creating empty AST node.
 */
#define __init_node_empty(type, parent) \
    fl_ast_node_init(&node, type, FL_NULL, parent, arr->current->location)

/**
 * @brief Shorthand for creating AST node filled with some value.
 */
#define __init_node_filled(type, value, parent) \
    fl_ast_node_init(&node, type, value, parent, arr->current->location)

// ---------- DECLARATIONS --------------

/**
 * @brief Sort operators using it's related ordering data.
 * @param parent Node from which to sort operators.
 */
fl_void _fl_ast_sort_operators(fl_ast_node *parent);

/**
 * @brief Process any kind of expression.
 * @param parent Node to which add resulting expression node.
 *               May be `FL_NULL` if this behaviour isn't wanted.
 * @param arr Token array to process from.
 * @param grouped Flag whether this expression is in group ( '(' ... ')' ).
 * @return Resulting expression node.
 */
fl_ast_node *_fl_ast_process_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool grouped);

/**
 * @brief Get operator sorting order.
 * @param op_node Operator node to use.
 * @param instr_infos Instruction info vector used to retrieve info about instructions order.
 * @return Operation sorting order.
 */
fl_op_order _fl_ast_get_op_order(const fl_ast_node *op_node, const fl_instr_info_vec *instr_infos);

/**
 * @brief `_fl_ast_sort_operators` helper function.
 * @param outer_node Outer operator node.
 * @param instr_infos Instruction info vector.
 * @return Operation sorting order.
 */
fl_op_order _fl_ast_sort_operators_helper(fl_ast_node *outer_node, const fl_instr_info_vec *instr_infos);

/**
 * @brief Process function call.
 * @param func_id Already parsed called function id.
 * @param arr Token array to process from.
 * @return Resulting AST node.
 */
fl_ast_node *_fl_ast_process_function_call(fl_ast_node *func_id, fl_token_arr *arr);

/**
 * @brief Process array.
 * @param arr Token array to process from.
 * @return Resulting array AST node.
 */
fl_ast_node *_fl_ast_process_array(fl_token_arr *arr);

/**
 * @brief Process operator.
 * @param parent Node to which add resulting node. May be `FL_NULL`.
 * @param arr Token array to process from.
 * @param left Already parsed left side of the operator.
 * @param grouped Flag whether this operator is grouped (in parentheses ()).
 * @return Resulting AST node.
 */
fl_ast_node *_fl_ast_process_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left, fl_bool grouped);

/**
 * @brief Process block ({}) with it's children.
 * @param parent Node to which add resulting node. May be `FL_NULL`.
 * @param arr Token array to process from.
 * @param is_root Flag whether this block is a root block.
 *        If `FL_TRUE` it has no braces.
 * @return Resulting AST block node.
 */
fl_ast_node *_fl_ast_process_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root);

/**
 * @brief Process identifier chain. Guarantees that next token is not null.
 * @param parent Node to which add resulting node. May be `FL_NULL`.
 * @param arr Token array to process from.
 * @return Resulting AST node.
 */
fl_ast_node *_fl_ast_process_id(fl_ast_node *parent, fl_token_arr *arr);

/**
 * @brief Process literal.
 * @param arr Token array to process from.
 * @return Resulting AST node.
 */
FL_INLINE_FN fl_ast_node *_fl_ast_process_ast_literal(fl_token_arr *arr);

// ---------- DEFINITIONS ---------------

fl_ast_node *fl_ast_gen(fl_token_arr *arr)
{
    fl_token_arr_reset_ptr(arr);

    // Assume the whole program is a block
    fl_ast_node *node = _fl_ast_process_block(FL_NULL, arr, FL_TRUE);

    _fl_ast_sort_operators(node);

    // TODO: check this when correct error handling will be implemented
    // execute errors if there are any, because it doesn't make much sense to go on with wrong syntax tree
    fl_err_execute();

    return node;
}

fl_op_order _fl_ast_get_op_order(const fl_ast_node *op_node, const fl_instr_info_vec *instr_infos)
{
    assert(op_node->type == FL_AST_NODE_TYPE_OPERATOR);
    assert(op_node->child[0].type == FL_AST_NODE_TYPE_ID_CHAIN);

    fl_id_chain *id_chain = op_node->child[0].data;
    fl_op_order order = FL_OP_ORDER_NEUTRAL;

    if (id_chain->size == 1)
    {
        // Assume it's instruction and try to find information about it
        fl_instr_info *info = fl_instr_info_vec_find(instr_infos, id_chain->data[0]);

        if (info)
            // It's an instruction, use it's sorting order
            return info->order;
    }

    return order;
}

fl_void _fl_ast_sort_operators(fl_ast_node *parent)
{
    const fl_instr_info_vec *infos = fl_g_instrs_get_info_vec();

    for (fl_ast_node *child = parent->child; child; child = child->next)
    {
        if (!child->child)
            break;

        if (child->type == FL_AST_NODE_TYPE_OPERATOR)
            _fl_ast_sort_operators_helper(child, infos);
        else
            _fl_ast_sort_operators(child);
    }
}

fl_op_order _fl_ast_sort_operators_helper(fl_ast_node *outer_node, const fl_instr_info_vec *instr_infos)
{
    assert(outer_node->type == FL_AST_NODE_TYPE_OPERATOR);

    fl_op_order outer_order = _fl_ast_get_op_order(outer_node, instr_infos);

    // Expose grouping info only when climbing higher
    fl_bool is_grouped = *(fl_bool *)outer_node->data;

    for (fl_ast_node *inner_node = outer_node->child->next; inner_node; inner_node = inner_node->next)
    {
        if (inner_node->type != FL_AST_NODE_TYPE_OPERATOR)
        {
            _fl_ast_sort_operators(inner_node);
            continue;
        }

        fl_op_order inner_order = _fl_ast_sort_operators_helper(inner_node, instr_infos);

        if (outer_order > inner_order)
        {
            // Detach nodes children
            fl_ast_node *outer_children = outer_node->child;
            fl_ast_node *inner_children = inner_node->child;
            fl_ast_node *mid_arg = fl_ast_node_detach(inner_children->next);
            outer_node->child = FL_NULL;
            inner_node->child = FL_NULL;

            // Swap children

            outer_children->next->next = FL_NULL;
            fl_ast_node_append_child(inner_node, outer_children);
            fl_ast_node_append_child(inner_node, mid_arg);

            fl_ast_node_append_child(outer_node, inner_children);
            fl_ast_node_insert_after(outer_node->child, inner_node);

            outer_order = _fl_ast_sort_operators_helper(inner_node, instr_infos);
        }
    }

    return is_grouped ? FL_OP_ORDER_GROUP : outer_order;
}

fl_ast_node *_fl_ast_process_id(fl_ast_node *parent, fl_token_arr *arr)
{
    // Init id chain
    fl_id_chain *chain = fl_mem_alloc_t(fl_id_chain);
    fl_id_chain_init(chain);

    while (arr->current->type == FL_TOKEN_TYPE_ID)
    {
        // Push sub-id
        fl_id_chain_push(chain, fl_str_dup(arr->current->value));

        // Check for end of code or chaining dot
        __next_token_safe;

        if (arr->current->alias != FL_TOKEN_ALIAS_DOT)
            break;

        // Skip remaining dot (move to next id)
        __next_token_safe;
    }

    fl_ast_node *node;
    __init_node_filled(FL_AST_NODE_TYPE_ID_CHAIN, chain, parent);

    return node;
}

fl_ast_node *_fl_ast_process_function_call(fl_ast_node *func_id, fl_token_arr *arr)
{
    fl_ast_node *node;
    __init_node_empty(FL_AST_NODE_TYPE_FN_CALL, FL_NULL);

    // Add id as a first child
    fl_ast_node_append_child(node, func_id);

    // Handle parameters

    // Expect lparen
    if (arr->current->alias != FL_TOKEN_ALIAS_LPAREN)
        __throw_syntax_error("Expected '(' when calling a function");

    do
    {
        __next_token_safe;
        if (arr->current->alias == FL_TOKEN_ALIAS_RPAREN)
            break;

        _fl_ast_process_expression(node, arr, FL_FALSE);
    } while (arr->current->alias == FL_TOKEN_ALIAS_COMMA);

    // expect rparen
    if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
        __throw_syntax_error("Missing ')' (closing of function call)");

    __next_token_safe;
    return node;
}

fl_ast_node *_fl_ast_process_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool grouped)
{
    fl_ast_node *node;

    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
    {
        // Inner expression in parenths
        __next_token_safe;
        node = _fl_ast_process_expression(FL_NULL, arr, FL_TRUE);
    }
    else if (arr->current->type == FL_TOKEN_TYPE_LITERAL)
        node = _fl_ast_process_ast_literal(arr);
    else if (arr->current->alias == FL_TOKEN_ALIAS_LBRACKET)
        // Array body
        node = _fl_ast_process_array(arr);
    // identifier can be either left side of the operator or a function name
    else if (arr->current->type == FL_TOKEN_TYPE_ID)
        node = _fl_ast_process_id(FL_NULL, arr);
    else
        __throw_syntax_error("Unexpected token");

    if (node->type == FL_AST_NODE_TYPE_ID_CHAIN)
    {
        // If lparen is next, it's a function
        if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
            node = _fl_ast_process_function_call(node, arr);
    }

    // If the token isn't a delim, assume it's operator
    if (!fl_token_is_delim(arr->current))
        node = _fl_ast_process_operator(parent, arr, node, grouped);
    // Else just add child to the parent if it's not null
    else if (parent)
        fl_ast_node_append_child(parent, node);

    if (grouped)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
            __throw_syntax_error("Missing closure of group ')'");
        else
            __next_token_safe;
    }

    return node;
}

fl_ast_node *_fl_ast_process_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root)
{
    fl_ast_node *node;

    // Create parent block node
    __init_node_empty(FL_AST_NODE_TYPE_BLOCK, parent);
    parent = node;

    // Check for brace if block is not a root
    if (!is_root)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_LBRACE)
            __throw_syntax_error("Expected block opening '{'");
        else
            __next_token_safe;
    }

    // Iterate over tokens
    for (fl_bool quit = FL_FALSE; !quit && arr->current; __next_token)
    {
        switch (arr->current->alias)
        {
        case FL_TOKEN_ALIAS_COMMENT:
        case FL_TOKEN_ALIAS_SEMICOLON:
            continue;
        case FL_TOKEN_ALIAS_LBRACE:
            // inner block to handle
            _fl_ast_process_block(parent, arr, FL_FALSE);

#if _INCLUDE_INLINE_LOGS

            printf("[DEBUG] AST generator passed block:\n");
            fl_prt_print_ast_tree(fl_ast_node_peek_child(parent));
            printf("\n");

#endif

            break;
        case FL_TOKEN_ALIAS_RBRACE:
            if (is_root)
                __throw_syntax_error("Unexpected block closure ('}')");

            quit = FL_TRUE;
            break;
        default:
            _fl_ast_process_expression(parent, arr, FL_FALSE);

            // throw error if expression isn't ending with a semicolon
            if (arr->current->alias != FL_TOKEN_ALIAS_SEMICOLON)
                __throw_syntax_error("expected semicolon ';' at the end of expression");

#if _INCLUDE_INLINE_LOGS

            printf("[DEBUG] AST generator passed expression at line %u:\n", arr->current->location.line + 1);
            fl_prt_print_ast_tree(fl_ast_node_peek_child(parent));
            printf("\n");

#endif

            break;
        }
    }

    return parent;
}

fl_ast_node *_fl_ast_process_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left, fl_bool grouped)
{
    fl_ast_node *node;

    // init operator data (info)
    fl_bool *data = fl_mem_alloc_t(fl_bool);
    *data = grouped;
    // create operator node
    __init_node_filled(FL_AST_NODE_TYPE_OPERATOR, data, FL_NULL);

    // add operator id node
    _fl_ast_process_id(node, arr);
    // add left side of the operator as a child
    fl_ast_node_append_child(node, left);

    if (arr->current->alias == FL_TOKEN_ALIAS_LBRACE)
        // handle block on the right side of the operator...
        _fl_ast_process_block(node, arr, FL_FALSE);
    else
        // ... else handle expression
        _fl_ast_process_expression(node, arr, FL_FALSE);

    // check for operators chaining
    if (arr->current && arr->current->type == FL_TOKEN_TYPE_ID)
    {
        node = _fl_ast_process_operator(parent, arr, node, grouped);
        *data = FL_FALSE;
    }
    else if (parent)
        fl_ast_node_append_child(parent, node);

    return node;
}

FL_INLINE_FN fl_ast_node *_fl_ast_process_ast_literal(fl_token_arr *arr)
{
    assert(arr->current->type == FL_TOKEN_TYPE_LITERAL);

    fl_ast_node *node;
    fl_value *literal_value = fl_mem_alloc_t(fl_value);

    *literal_value = fl_value_from_literal(arr->current->alias, arr->current->value);

    __init_node_filled(FL_AST_NODE_TYPE_LITERAL, literal_value, FL_NULL);

    __next_token_safe;
    return node;
}

fl_ast_node *_fl_ast_process_array(fl_token_arr *arr)
{
    fl_ast_node *node;
    __init_node_empty(FL_AST_NODE_TYPE_ARRAY, FL_NULL);

    if (arr->current->alias != FL_TOKEN_ALIAS_LBRACKET)
        assert(FL_FALSE);

    do
    {
        __next_token_safe;
        if (arr->current->alias == FL_TOKEN_ALIAS_RBRACKET)
            break;

        _fl_ast_process_expression(node, arr, FL_FALSE);
    } while (arr->current->alias == FL_TOKEN_ALIAS_COMMA);

    if (arr->current->alias != FL_TOKEN_ALIAS_RBRACKET)
        assert(FL_FALSE);
    else
        __next_token_safe;

    return node;
}