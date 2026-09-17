/**
 * ---------- ast_tree.c -----------------------------
 *
 * Implements general Abstract Syntax Tree logic.
 *
 * ---------------------------------------------------
 */

#include "ast_tree.h"

// ---------- DEPENDENCIES -----------

#include "shared/error.h"
#include "shared/str.h"
#include "flow/id/id_chain.h"
#include "flow/value.h"
#include "tokenizer/token_location.h"
#include "tokenizer/token_arr.h"
#include "vecs/instr_info_vec.h"
#include "ast/ast_node.h"
#include "flow/instr/g_instrs.h"

#ifndef NDEBUG

#include "tools/printer.h"
#include <stdio.h>

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
    fl_err_throw(FL_ERR_AST, "Unexpected end of code", arr->_data[arr->_iterator].value, arr->_data[arr->_iterator].location)

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
 * @brief Sorts operators using it's related ordering data.
 * @param parent Node from which to sort operators.
 */
fl_void _fl_ast_sort_operators(fl_ast_node *parent);

/**
 * @brief Processes any kind of expression.
 * @param parent Node to which add resulting expression nodes.
 *               May be `FL_NULL` if this behaviour isn't wanted.
 * @param arr Token array to process from.
 * @param grouped Flag whether this expression is in group ( '(' ... ')' ).
 * @return Resulting expression node.
 */
fl_ast_node *_fl_ast_process_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool grouped);

/**
 * @brief Gets operator sorting order.
 * @param op_node Operator node to use.
 * @param instr_infos Instruction info vector used to retrieve info about instructions order.
 * @return Operation sorting order.
 */
fl_op_order _fl_ast_get_op_order(const fl_ast_node *op_node, const fl_instr_info_vec *instr_infos);

/**
 * @brief
 * @param outer_node
 * @param instr_infos
 * @return
 */
fl_op_order _fl_ast_sort_operators_helper(fl_ast_node *outer_node, const fl_instr_info_vec *instr_infos);
fl_ast_node *_fl_ast_process_function_call(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *func_id);
fl_ast_node *_fl_ast_process_array_literal(fl_ast_node *parent, fl_token_arr *arr);
fl_ast_node *_fl_ast_process_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left, fl_bool grouped);
fl_ast_node *_fl_ast_process_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root);
fl_ast_node *_fl_ast_process_id(fl_ast_node *parent, fl_token_arr *arr);
inline fl_ast_node *_fl_ast_process_ast_literal(fl_ast_node *parent, fl_token_arr *arr);

// ---------- DEFINITIONS ---------------

fl_ast_node *fl_ast_tree_gen(fl_token_arr *arr)
{
    fl_token_arr_reset_ptr(arr);

    // Assume the whole program is a block
    fl_ast_node *node = _fl_ast_process_block(FL_NULL, arr, FL_TRUE);

    _fl_ast_sort_operators(node);

    // TODO: check this when correct error handling will be implemented
    // execute errors if there are any,
    // because it doesn't make much sense to go on with wrong syntax tree
    fl_err_execute();

    return node;
}

fl_op_order _fl_ast_get_op_order(const fl_ast_node *op_node, const fl_instr_info_vec *instr_infos)
{
    assert(op_node->type == FL_AST_NODE_TYPE_OPERATOR);

    fl_bool is_grouped = *(fl_bool *)op_node->data;
    fl_op_order order = is_grouped ? FL_OP_ORDER_GROUP : FL_OP_ORDER_NEUTRAL;

    assert(op_node->child[0].type == FL_AST_NODE_TYPE_ID_CHAIN);
    fl_id_chain *id_chain = op_node->child[0].data;

    if (!is_grouped && id_chain->size == 1)
    {
        fl_instr_info *info = fl_instr_info_vec_find(instr_infos, id_chain->data[0]);

        if (info)
            // It's an instruction, use it's order
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

    for (fl_ast_node *inner_node = outer_node->child->next; inner_node; inner_node = inner_node->next)
    {
        if (inner_node->type != FL_AST_NODE_TYPE_OPERATOR)
            continue;

        fl_op_order inner_order = _fl_ast_sort_operators_helper(inner_node, instr_infos);

        if (outer_order > inner_order)
        {
            // swap operator nodes

            _fl_swap(fl_ast_node *, outer_node->parent, inner_node->parent);
            _fl_swap(fl_ast_node *, outer_node->child, inner_node->child);

            fl_ast_node *outer_last = outer_node->child->next->next;
            fl_ast_node *inner_last = inner_node->child->next->next;
            outer_node->child->next->next = inner_node;
            inner_node->next = FL_NULL;
            inner_node->child->next = outer_last;
            outer_last->next = inner_last;

            return _fl_ast_sort_operators_helper(inner_node, instr_infos);
        }
    }

    return outer_order;
}

// handles id chain (eg. 'body.hand')
// guarantees that next token is not null
fl_ast_node *_fl_ast_process_id(fl_ast_node *parent, fl_token_arr *arr)
{
    // init id chain
    fl_id_chain *chain = fl_mem_alloc_t(fl_id_chain);
    fl_id_chain_init(chain);

    while (arr->current->type == FL_TOKEN_TYPE_ID)
    {
        // push sub-id
        fl_id_chain_push(chain, fl_str_dup(arr->current->value));

        // check for end of code or chaining dot
        __next_token_safe;

        if (arr->current->alias != FL_TOKEN_ALIAS_DOT)
            break;

        // skip remaining dot (move to next id)
        __next_token_safe;
    }

    fl_ast_node *node;
    __init_node_filled(FL_AST_NODE_TYPE_ID_CHAIN, chain, parent);
    return node;
}

fl_ast_node *_fl_ast_process_function_call(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *func_id)
{
    fl_ast_node *node;
    __init_node_empty(FL_AST_NODE_TYPE_FN_CALL, parent);
    parent = node;

    // add id as a first child
    fl_ast_node_add_child(parent, func_id);

    // handle parameters

    // expect lparen
    if (arr->current->alias != FL_TOKEN_ALIAS_LPAREN)
        __throw_syntax_error("Expected '(' when calling a function");

    do
    {
        __next_token_safe;
        if (arr->current->alias == FL_TOKEN_ALIAS_RPAREN)
            break;

        _fl_ast_process_expression(parent, arr, FL_FALSE);
    } while (arr->current->alias == FL_TOKEN_ALIAS_COMMA);

    // expect rparen
    if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
        __throw_syntax_error("Missing ')' (closing of function call)");

    __next_token_safe;
    return parent;
}

fl_ast_node *_fl_ast_process_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool grouped)
{
    // temp node
    fl_ast_node *node;

    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
    {
        // inner expression in parenths
        __next_token_safe;
        node = _fl_ast_process_expression(FL_NULL, arr, FL_TRUE);
    }
    else if (arr->current->type == FL_TOKEN_TYPE_LITERAL)
        node = _fl_ast_process_ast_literal(FL_NULL, arr);
    else if (arr->current->alias == FL_TOKEN_ALIAS_LBRACKET)
        // Array literal
        node = _fl_ast_process_array_literal(FL_NULL, arr);
    // identifier can be either left side of the operator or a function name
    else if (arr->current->type == FL_TOKEN_TYPE_ID)
        node = _fl_ast_process_id(FL_NULL, arr);
    else
        __throw_syntax_error("Unexpected token");

    if (node->type == FL_AST_NODE_TYPE_ID_CHAIN)
    {
        // if lparen is next, it's a function
        if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
            node = _fl_ast_process_function_call(FL_NULL, arr, node);
    }

    // check if it is the end of expression - try to add this node to the parent
    if (parent && (fl_token_is_delim(arr->current) || parent->type == FL_AST_NODE_TYPE_OPERATOR))
        fl_ast_node_add_child(parent, node);
    else
        node = _fl_ast_process_operator(parent, arr, node, grouped);

    if (grouped)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
            __throw_syntax_error("Missing closure of group ')'");
        else
            __next_token_safe;
    }

    return node;
}

// handles single block ({...}) of code and its child
fl_ast_node *_fl_ast_process_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root)
{
    // temp variable
    fl_ast_node *node;

    // create parent node (block node)
    __init_node_empty(FL_AST_NODE_TYPE_BLOCK, parent);
    parent = node;

    // check for brace if block is not root
    if (!is_root)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_LBRACE)
            __throw_syntax_error("Expected block opening '{'");
        else
            __next_token_safe;
    }

    // iterate over tokens
    fl_bool quit = FL_FALSE;

    for (; !quit && arr->current; __next_token)
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

            // is it ok?

            // throw error if expression isn't ending with a semicolon
            if (arr->current->alias != FL_TOKEN_ALIAS_SEMICOLON)
                __throw_syntax_error("expected semicolon ';' at the end of expression");

#if _INCLUDE_INLINE_LOGS
            printf("[DEBUG] AST generator passed expression at line %lu:\n", arr->current->location.line + 1);
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
    fl_ast_node_add_child(node, left);

    if (arr->current->alias == FL_TOKEN_ALIAS_LBRACE)
        // handle block on the right side of the operator...
        _fl_ast_process_block(node, arr, FL_FALSE);
    else
        // ... else handle expression
        _fl_ast_process_expression(node, arr, FL_FALSE);

    // check for operators chaining (?)
    if (arr->current && arr->current->type == FL_TOKEN_TYPE_ID)
        node = _fl_ast_process_operator(parent, arr, node, FL_FALSE);
    else if (parent)
        fl_ast_node_add_child(parent, node);

    return node;
}

inline fl_ast_node *_fl_ast_process_ast_literal(fl_ast_node *parent, fl_token_arr *arr)
{
    assert(arr->current->type == FL_TOKEN_TYPE_LITERAL);

    fl_ast_node *node;
    fl_value *literal_value = fl_mem_alloc_t(fl_value);
    *literal_value = fl_value_from_literal(arr->current->alias, arr->current->value);

    __init_node_filled(FL_AST_NODE_TYPE_LITERAL, literal_value, parent);

    __next_token_safe;
    return node;
}

fl_ast_node *_fl_ast_process_array_literal(fl_ast_node *parent, fl_token_arr *arr)
{
    fl_ast_node *node;
    __init_node_empty(FL_AST_NODE_TYPE_ARRAY, parent);

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