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
#include "ast/ast_node.h"
// may be to remove
#include "revise_me/info/operator_info.h"

#ifndef NDEBUG
#include "tools/printer.h"
#include <stdio.h>
#endif

// ---------- MACROS ----------------

/**
 * @brief Shorthand for throwing syntax error.
 */
#define throw_syntax_error(message) \
    fl_err_throw(FL_ERR_AST, message, arr->current->value, arr->current->location)

/**
 * @brief Shorthand for incrementing token pointer.
 */
#define next_token \
    fl_token_arr_next(arr)

/**
 * @brief Shorthand for incrementing token pointer, and throwing an error if it's end of code.
 */
#define next_token_safe \
    if (!next_token)    \
    fl_err_throw(FL_ERR_AST, "Unexpected end of code", arr->_data[arr->_iterator].value, arr->_data[arr->_iterator].location)

/**
 * @brief Shorthand for creating empty AST node.
 */
#define init_node_empty(type, parent) \
    fl_ast_node_init(&node, type, FL_NULL, parent, arr->current->location)

/**
 * @brief Shorthand for creating AST node filled with some value.
 */
#define init_node_filled(type, value, parent) \
    fl_ast_node_init(&node, type, value, parent, arr->current->location)

// ---------- DECLARATIONS --------------

inline fl_ast_node *_fl_ast_process_ast_literal(fl_ast_node *parent, fl_token_arr *arr);
inline fl_ast_node *_fl_ast_process_function_call(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *func_id);
inline fl_ast_node *_fl_ast_process_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left, fl_bool grouped);
inline fl_ast_node *_fl_ast_process_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root);
inline fl_ast_node *_fl_ast_process_id(fl_ast_node *parent, fl_token_arr *arr);
fl_void _fl_ast_process_function_header(fl_ast_node *parent, fl_token_arr *arr);
fl_void _fl_ast_process_function_expression(fl_ast_node *parent, fl_token_arr *arr);
fl_ast_node *_fl_ast_process_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool grouped);

// ---------- DEFINITIONS ---------------

fl_ast_node *fl_ast_tree_gen(fl_token_arr *arr)
{
    fl_token_arr_reset_ptr(arr);

    // we assume that the whole program is a block
    fl_ast_node *node = _fl_ast_process_block(FL_NULL, arr, FL_TRUE);

    // TODO: check this when correct error handling will be implemented
    // execute errors if there are any,
    // because it doesn't make much sense to go on with wrong syntax tree
    fl_err_execute();

    return node;
}

// handles id chain (eg. 'body.hand')
// guarantees that next token is not null
inline fl_ast_node *_fl_ast_process_id(fl_ast_node *parent, fl_token_arr *arr)
{
    // init id chain
    fl_id_chain *chain = fl_mem_alloc_t(fl_id_chain);
    fl_id_chain_init(chain);

    while (arr->current->type == FL_TOKEN_TYPE_ID)
    {
        // push sub-id
        fl_id_chain_push(chain, fl_str_dup(arr->current->value));

        // check for end of code or chaining dot
        next_token_safe;

        if (arr->current->alias != FL_TOKEN_ALIAS_DOT)
            break;

        // skip remaining dot (move to next id)
        next_token_safe;
    }

    fl_ast_node *node;
    init_node_filled(FL_AST_NODE_TYPE_ID_CHAIN, chain, parent);
    return node;
}

inline fl_ast_node *_fl_ast_process_function_call(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *func_id)
{
    fl_ast_node *node;
    init_node_empty(FL_AST_NODE_TYPE_FN_CALL, parent);
    parent = node;

    // add id as a first child
    fl_ast_node_add_child(parent, func_id);

    // handle parameters

    // expect lparen
    if (arr->current->alias != FL_TOKEN_ALIAS_LPAREN)
        throw_syntax_error("Expected '(' when calling a function");

    do
    {
        next_token_safe;
        _fl_ast_process_expression(parent, arr, FL_FALSE);
    } while (arr->current->alias == FL_TOKEN_ALIAS_COMMA);

    // expect rparen
    if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
        throw_syntax_error("Missing ')' (closing of function call)");

    next_token_safe;
    return parent;
}

// handles any kind of expression
// almost everything is an expression (apart from blocks)
// function may contain a single expression, which is 'instantiated' on call
// it can be a ast_literal, function call, group of operators or a combination of these things
fl_ast_node *_fl_ast_process_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool grouped)
{
    // temp node
    fl_ast_node *node;

    // inner expression in parenths
    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
    {
        next_token_safe;
        node = _fl_ast_process_expression(FL_NULL, arr, FL_TRUE);
    }
    else if (arr->current->type == FL_TOKEN_TYPE_LITERAL)
        node = _fl_ast_process_ast_literal(FL_NULL, arr);
    // identifier can be either left side of the operator or a function name
    else if (arr->current->type == FL_TOKEN_TYPE_ID)
        node = _fl_ast_process_id(FL_NULL, arr);
    else
        throw_syntax_error("Unexpected token");

    if (node->type == FL_AST_NODE_TYPE_ID_CHAIN)
    {
        // if lparen is next, it's a function
        if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
            node = _fl_ast_process_function_call(FL_NULL, arr, node);
    }

    // check if it is the end of expression
    if (fl_token_is_delim(arr->current))
        fl_ast_node_add_child(parent, node);
    else if (!parent || parent->type != FL_AST_NODE_TYPE_OPERATOR)
        node = _fl_ast_process_operator(parent, arr, node, grouped);
    else
        fl_ast_node_add_child(parent, node);

    if (grouped)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
            throw_syntax_error("Missing closure of group ')'");
        else
            next_token_safe;
    }

    return node;
}

// handles single block ({...}) of code and its child
inline fl_ast_node *_fl_ast_process_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root)
{
    // temp variable
    fl_ast_node *node;

    // create parent node (block node)
    init_node_empty(FL_AST_NODE_TYPE_BLOCK, parent);
    parent = node;

    // check for brace if block is not root
    if (!is_root)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_LBRACE)
            throw_syntax_error("Expected block opening '{'");
        else
            next_token_safe;
    }

    // iterate over tokens
    for (; arr->current; next_token)
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
                throw_syntax_error("Unexpected block closure ('}')");
            break;
        default:
            _fl_ast_process_expression(parent, arr, FL_FALSE);

            // throw error if expression isn't ending with a semicolon
            if (arr->current->alias != FL_TOKEN_ALIAS_SEMICOLON)
                throw_syntax_error("expected semicolon ';' to end expression");

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

inline fl_ast_node *_fl_ast_process_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left, fl_bool grouped)
{
    fl_ast_node *node;

    // create operator info
    fl_operator_info *op_info;
    fl_operator_info_init(&op_info, grouped ? FL_OPR_ORDER_MAX : FL_OPR_ORDER_DEFAULT);
    // create operator node
    init_node_filled(FL_AST_NODE_TYPE_OPERATOR, op_info, FL_NULL);

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

    if (arr->current->type == FL_TOKEN_TYPE_ID)
        node = _fl_ast_process_operator(parent, arr, node, FL_FALSE);
    else if (parent)
        fl_ast_node_add_child(parent, node);

    return node;
}

fl_void _fl_ast_process_function_header(fl_ast_node *parent, fl_token_arr *arr)
{
    assert(parent->type == FL_AST_NODE_TYPE_FUNCTION);

    // TODO

    // fl_ast_node *statement;
    fl_ast_node *node;
    fl_bool parenth = FL_FALSE;

    init_node_empty(FL_AST_NODE_TYPE_FUNC_HEADER, parent);
    parent = node;

    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
    {
        parenth = FL_TRUE;
        next_token_safe;
    }

    do
    {
        // parameter id definition

        if (arr->current->type != FL_TOKEN_TYPE_ID)
        {
            // throw_syntax_error(, arr->current);
        }

        // fl_ast_node_init(&statement, FL_AST_NODE_TYPE_STATEMENT, FL_NULL, parent);
        // init_node_filled(FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(arr->current->value), statement);

        next_token_safe;

        // TODO: to revise
        // if (arr->current.alias == FL_TOKEN_ALIAS_COLON)
        // {
        //     // parameter type definition

        //     strong_increment;
        //     _fl_ast_process_type_dimensions(statement, arr);
        // }
    } while (arr->current->alias == FL_TOKEN_ALIAS_COMMA);

    if (parenth)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
        {
            // throw_syntax_error(, arr->current);
        }
        else
        {
            next_token_safe;
        }
    }
}

fl_void _fl_ast_process_function_expression(fl_ast_node *parent, fl_token_arr *arr)
{
    fl_ast_node *func_node;

    fl_ast_node_init(&func_node, FL_AST_NODE_TYPE_FUNCTION, FL_NULL, parent, arr->current->location);

    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN || arr->current->type == FL_TOKEN_TYPE_ID)
        _fl_ast_process_function_header(func_node, arr);

    if (arr->current->alias == FL_TOKEN_ALIAS_LBRACE)
        // function block
        _fl_ast_process_block(func_node, arr, FL_FALSE);
    else
    {
        // single expression
        fl_ast_node *block_node;

        fl_ast_node_init(&block_node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, parent, arr->current->location);

        _fl_ast_process_expression(block_node, arr, FL_FALSE);
    }
}

inline fl_ast_node *_fl_ast_process_ast_literal(fl_ast_node *parent, fl_token_arr *arr)
{
    assert(arr->current->type == FL_TOKEN_TYPE_LITERAL);

    fl_ast_node *node;
    fl_value *literal_value = fl_mem_alloc_t(fl_value);
    *literal_value = fl_value_from_literal(arr->current->alias, arr->current->value);

    init_node_filled(FL_AST_NODE_TYPE_LITERAL, literal_value, parent);

    next_token_safe;
    return node;
}
