#include "compiler/ast.h"

#include "shared/token.h"
#include "shared/token_array.h"
#include "shared/memory.h"
#include "shared/error.h"

#ifndef NDEBUG
#include "tools/printer.h"
#include <stdio.h>
#endif

// --- DEFINES ---

#define throw_syntax_error(message) \
    fl_err_throw(FL_ERR_AST, message, *arr->current)

#define next_token \
    fl_token_arr_next(arr)

#define next_token_safe \
    if (!next_token)    \
    fl_err_throw(FL_ERR_AST, "Unexpected end of code", arr->_data[arr->_iterator])

#define init_node_empty(type, parent) \
    fl_ast_node_init(&node, type, FL_NULL, parent, arr->current->location);

#define init_node_filled(type, value, parent) \
    fl_ast_node_init(&node, type, value, parent, arr->current->location);

// --- STATIC FUNCTIONS DEFINITION ---

inline static fl_ast_node *_fl_ast_handle_function_call(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *func_id);
inline static fl_void _fl_ast_handle_type_dimensions(fl_ast_node *type_id, fl_token_arr *arr);
inline static fl_ast_node *_fl_ast_handle_id(fl_ast_node *parent, fl_token_arr *arr);
inline static fl_ast_node *_fl_ast_handle_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left);
static fl_void _fl_ast_handle_function_header(fl_ast_node *parent, fl_token_arr *arr);
static fl_void _fl_ast_handle_function_expression(fl_ast_node *parent, fl_token_arr *arr);
static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root);
static fl_ast_node *_fl_ast_handle_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool inner);

// --- IMPLEMENTATIONS ---

// handles id specifier (eg. 'body.hand')
// guarantees that next token is not null
inline static fl_ast_node *_fl_ast_handle_id(fl_ast_node *parent, fl_token_arr *arr)
{
    fl_ast_node *node;

    // init parent (id chain) node
    init_node_empty(FL_AST_NODE_TYPE_ID_CHAIN, parent);
    parent = node;

    // iterate over tokens
    while (arr->current->type == FL_TOKEN_TYPE_ID)
    {
        // create id node
        init_node_filled(FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(arr->current->value), parent);

        // check for end of code or chaining dot
        if (!next_token || arr->current->alias != FL_TOKEN_ALIAS_DOT)
            break;

        // skip remaining dot (move to next id)
        next_token_safe;
    }

    // id chain handler failed
    if (parent->children == FL_NULL)
        throw_syntax_error("Expected id chain");

    return parent;
}

inline static fl_ast_node *_fl_ast_handle_function_call(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *func_id)
{
    fl_ast_node *node;
    init_node_empty(FL_AST_NODE_TYPE_CALL, parent);
    parent = node;

    // add id as a first child
    fl_ast_node_add_child(parent, func_id);

    // handle parameters

    // expect lparen
    if (arr->current->alias != FL_TOKEN_ALIAS_LPAREN)
        throw_syntax_error("Expected '(' when calling a function");

    next_token_safe;

    do
    {
        _fl_ast_handle_expression(parent, arr, FL_FALSE);
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
// it can be a literal, function call, group of operators or a combination of these things
static fl_ast_node *_fl_ast_handle_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool inner)
{
    // temp node
    fl_ast_node *node;

    // inner expression in parenths
    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
    {
        next_token_safe;
        node = _fl_ast_handle_expression(FL_NULL, arr, FL_TRUE);
    }
    else if (arr->current->type == FL_TOKEN_TYPE_LITERAL)
    {
        fl_ast_literal_data *literal_info;
        fl_ast_literal_data_init(&literal_info, arr->current->alias, arr->current->value);
        init_node_filled(FL_AST_NODE_TYPE_LITERAL, literal_info, FL_NULL);

        next_token_safe;
    }
    // identifier can be either left side of the operator or a function name
    else if (arr->current->type == FL_TOKEN_TYPE_ID)
        node = _fl_ast_handle_id(FL_NULL, arr);
    else
        throw_syntax_error("Unexpected token");

    if (node->type == FL_AST_NODE_TYPE_ID_CHAIN)
    {
        // if lparen is next, it's a function
        if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
            node = _fl_ast_handle_function_call(FL_NULL, arr, node);
        // lbrace after id means that we have a dimensions qualifier or array access
        else if (arr->current->alias == FL_TOKEN_ALIAS_LBRACKET)
        {
            /// ???????????????????
            // next_token_safe;
            // // if next arr->current is RBRACE it has to be dimensions qualifier
            // if (arr->current.alias == FL_TOKEN_ALIAS_RBRACKET)
            // {
            //     (*i)--;
            //     _fl_ast_handle_type_dimensions(node, arr);
            // }
        }
    }

    // check if it is the end of expression
    if (fl_token_is_delim(arr->current))
        fl_ast_node_add_child(parent, node);
    else
        node = _fl_ast_handle_operator(parent, arr, node);

    if (inner)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
            throw_syntax_error("Missing closure of group ')'");
        else
            next_token_safe;
    }

    return node;
}

// handles single block ({...}) of code and its children
static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root)
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
        {
            throw_syntax_error("Expected block opening '{'");
        }
        else
            next_token_safe;
    }

    // iterate over tokens
    while (arr->current)
    {
        if (arr->current->alias == FL_TOKEN_ALIAS_SEMICOLON)
            next_token;
        // inner block to handle
        else if (arr->current->alias == FL_TOKEN_ALIAS_LBRACE)
        {
            _fl_ast_handle_block(parent, arr, FL_FALSE);

#if _INCLUDE_INLINE_LOGS
            printf("[DEBUG] AST generator passed block:\n");
            fl_prt_print_ast_tree(fl_ast_node_peek_child(parent), 0);
            printf("\n");
#endif
        }
        else if (arr->current->alias == FL_TOKEN_ALIAS_RBRACE)
        {
            if (is_root)
                throw_syntax_error("Unexpected block closure ('}')");
            else
                next_token;
        }
        else
        {
            _fl_ast_handle_expression(parent, arr, FL_FALSE);

            // throw error if expression isn't ending with a semicolon
            if (arr->current->alias == FL_TOKEN_ALIAS_SEMICOLON)
                next_token;
            else
                throw_syntax_error("expected semicolon ';' before to end expression");

#if _INCLUDE_INLINE_LOGS
            printf("[DEBUG] AST generator passed expression at line %lu:\n", arr->current->line + 1);
            fl_prt_print_ast_tree(fl_ast_node_peek_child(parent), 0);
            printf("\n");
#endif
        }
    }
}

inline static fl_ast_node *_fl_ast_handle_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left)
{
    fl_ast_node *node;

    // create operator node
    init_node_empty(FL_AST_NODE_TYPE_OPERATOR, parent);

    // add id node
    _fl_ast_handle_id(node, arr);
    // add left side of the operator as a child
    fl_ast_node_add_child(node, left);

    if (arr->current->alias == FL_TOKEN_ALIAS_LBRACE)
    {
        // handle block on the right side of the operator...
        _fl_ast_handle_block(node, arr, FL_FALSE);
    }
    else
        // ... else handle expression
        _fl_ast_handle_expression(node, arr, FL_FALSE);

    return node;
}

static fl_void _fl_ast_handle_function_header(fl_ast_node *parent, fl_token_arr *arr)
{
    assert(parent->type == FL_AST_NODE_TYPE_FUNCTION);

    // TODO

    fl_ast_node *statement;
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
        init_node_filled(FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(arr->current->value), statement);

        next_token_safe;

        // TODO: to revise
        // if (arr->current.alias == FL_TOKEN_ALIAS_COLON)
        // {
        //     // parameter type definition

        //     strong_increment;
        //     _fl_ast_handle_type_dimensions(statement, arr);
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

static fl_void _fl_ast_handle_function_expression(fl_ast_node *parent, fl_token_arr *arr)
{
    fl_ast_node *func_node;

    fl_ast_node_init(&func_node, FL_AST_NODE_TYPE_FUNCTION, FL_NULL, parent, arr->current->location);

    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN || arr->current->type == FL_TOKEN_TYPE_ID)
    {
        _fl_ast_handle_function_header(func_node, arr);
    }

    if (arr->current->alias == FL_TOKEN_ALIAS_LBRACE)
    {
        // function block
        _fl_ast_handle_block(func_node, arr, FL_FALSE);
    }
    else
    {
        // single expression
        fl_ast_node *block_node;

        fl_ast_node_init(&block_node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, parent, arr->current->location);

        _fl_ast_handle_expression(block_node, arr, FL_FALSE);
    }
}

// handles type dimensions
inline static fl_void _fl_ast_handle_type_dimensions(fl_ast_node *type_id, fl_token_arr *arr)
{
    // handle array dimensions
    fl_uint dimensions = 0;
    for (; arr->current->alias == FL_TOKEN_ALIAS_LBRACKET; dimensions++)
    {
        next_token_safe;

        // throw error if bracket is not closed
        if (arr->current->alias != FL_TOKEN_ALIAS_RBRACKET)
            throw_syntax_error("Wrong use of type dimensions specifier");

        next_token_safe;
        dimensions++;
    }

    // create type descriptor
    fl_ast_type_data *type_info;
    fl_ast_type_data_init(&type_info, dimensions);
    type_id->data = type_info;
}

// tries to generate abstract syntax tree from specified tokens
fl_ast_node *fl_ast_gen(fl_token_arr *arr)
{
    fl_token_arr_reset_ptr(arr);

    // create root node with no related data
    fl_ast_node *node;
    init_node_empty(FL_AST_NODE_TYPE_ROOT, FL_NULL);

    // we assume that the whole program is a block
    _fl_ast_handle_block(node, arr, FL_TRUE);

    return node;
}

fl_void fl_ast_free(fl_ast_node *node)
{
    if (!node)
        return;

    // Free children recursively
    fl_ast_node *child = node->children;
    while (child)
    {
        fl_ast_node *next_child = child->next;
        fl_ast_free(child);
        child = next_child;
    }

    // Free current (root) node
    fl_mem_free(node->data);
    fl_mem_free(node);
}

fl_void fl_ast_type_data_init(fl_ast_type_data **info, fl_uint dimensions)
{
    fl_ast_type_data *i = fl_mem_alloc(sizeof(fl_ast_type_data));

    i->dimensions = dimensions;

    *info = i;
}

fl_void fl_ast_node_init(fl_ast_node **node, fl_ast_node_type type, fl_unknown *data, fl_ast_node *parent, fl_token_location location)
{
    fl_ast_node *n = fl_mem_alloc(sizeof(fl_ast_node));

    n->type = type;
    n->data = data;
    n->children = FL_NULL;
    n->next = FL_NULL;
    n->location = location;

    if (parent)
        fl_ast_node_add_child(parent, n);
    else
        n->parent = FL_NULL;

    *node = n;
}

fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent)
{
    fl_ast_node *node = parent->children;
    while (node->next)
        node = node->next;
    return node;
}

fl_void fl_ast_node_add_child(fl_ast_node *parent, fl_ast_node *node)
{
    if (!parent->children)
    {
        node->parent = parent;
        parent->children = node;
        return;
    }

    fl_ast_node *current = parent->children;

    while (current->next)
        current = current->next;

    node->parent = parent;
    current->next = node;
}

fl_void fl_ast_type_data_free(fl_ast_type_data *info)
{
    fl_mem_free(info);
}

fl_void fl_ast_literal_data_init(fl_ast_literal_data **info, fl_ast_literal_type type, fl_unknown *value)
{
    fl_ast_literal_data *i = fl_mem_alloc(sizeof(fl_ast_literal_data));

    i->literal_type = type;
    i->data = value;

    *info = i;
}

fl_void fl_ast_literal_data_free(fl_ast_literal_data *info)
{
    fl_mem_free(info);
}