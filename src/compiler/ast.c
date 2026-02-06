#include "compiler/ast.h"

#include "shared/token.h"
#include "shared/token_array.h"
#include "shared/memory.h"
#include "shared/error.h"

// --- DEFINES ---

#define throw_syntax_error(message) \
    fl_err_throw(FL_ERR_AST_SYNTAX, message, *arr->current);

#define next_token \
    fl_token_arr_next(arr)

// shorthand for increasing current token index with error throw if it fails
#define next_token_with_assert \
    if (!next_token)           \
        fl_err_throw(FL_ERR_AST_SYNTAX, "Unexpected end of code", arr->_data[arr->_iterator]);

// --- STATIC FUNCTIONS DEFINITION ---

inline static fl_ast_node *_fl_ast_handle_function_call(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *func_id);
inline static fl_void _fl_ast_handle_type_dimensions(fl_ast_node *type_id, fl_token_arr *arr);
static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root);
static fl_ast_node *_fl_ast_handle_expression(fl_ast_node *parent, fl_token_arr *arr, fl_bool inner);
static fl_void _fl_ast_handle_function_header(fl_ast_node *parent, fl_token_arr *arr);
static fl_void _fl_ast_handle_function_expression(fl_ast_node *parent, fl_token_arr *arr);
static fl_ast_node *_fl_ast_handle_id_chain(fl_ast_node *parent, fl_token_arr *arr);
static fl_ast_node *_fl_ast_handle_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left);

// --- IMPLEMENTATIONS ---

// handles id chain (eg. '.body.hand')
static fl_ast_node *_fl_ast_handle_id_chain(fl_ast_node *parent, fl_token_arr *arr)
{
    fl_ast_node *node;

    // init parent (id chain) node
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_ID_CHAIN, FL_NULL, parent);
    parent = node;

    // dot at the beginning means access to global context
    // so we mark it as a flag
    if (arr->current->alias == FL_TOKEN_ALIAS_DOT)
    {
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_FLAG, FL_NULL, parent);
        next_token;
    }

    // iterate over tokens
    while (arr->current->type == FL_TOKEN_TYPE_IDENTIFIER)
    {
        // create id node
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(arr->current->value), parent);

        // check for end of code or chaining dot
        if (!next_token || arr->current->alias != FL_TOKEN_ALIAS_DOT)
            break;

        // skip remaining dot (move to next id)
        next_token_with_assert;
    }

    // id chain handler failed
    if (parent->children == FL_NULL)
        throw_syntax_error("Expected id chain");

    // consume space which separates id chains if it exists
    if (arr->current->alias == FL_TOKEN_ALIAS_SPACE)
        next_token_with_assert;

    return parent;
}

inline static fl_ast_node *_fl_ast_handle_function_call(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *func_id)
{
    fl_ast_node *node;
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_CALL, FL_NULL, parent);
    parent = node;

    // add id as a first child
    fl_ast_node_add_child(parent, func_id);

    // handle parameters

    // expect lparen
    if (arr->current->alias != FL_TOKEN_ALIAS_LPAREN)
        throw_syntax_error("Expected '(' when calling a function");

    next_token_with_assert;

    do
    {
        _fl_ast_handle_expression(parent, arr, FL_FALSE);
    } while (arr->current->alias == FL_TOKEN_ALIAS_COMMA);

    // expect rparen
    if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
        throw_syntax_error("Missing ')' (closing of function call)");

    next_token;
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
        next_token_with_assert;
        node = _fl_ast_handle_expression(FL_NULL, arr, FL_TRUE);
    }

    if (arr->current->type == FL_TOKEN_TYPE_LITERAL)
    {
        fl_ast_literal_info *literal_info;
        fl_ast_literal_info_init(&literal_info, arr->current->alias, arr->current->value);
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_LITERAL, literal_info, FL_NULL);

        // unexpected error may occure here?
        next_token_with_assert;

        // check if it is the end of expression
        if (!fl_token_is_delim(arr->current))
            // operator has to be next since it is an literal
            _fl_ast_handle_operator(parent, arr, node);
        else
            fl_ast_node_add_child(parent, node);
    }
    // identifier can be either left side of the operator or a function name
    else if (fl_token_is_id_like(arr->current))
    {
        node = _fl_ast_handle_id_chain(FL_NULL, arr);

        // check if there are any tokens remaining
        if (!arr->current)
        {
            fl_ast_node_add_child(parent, node);
            goto __return;
        }

        // if lparen is next, it's a function
        if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
            node = _fl_ast_handle_function_call(FL_NULL, arr, node);
        // lbrace after id means that we have a dimensions qualifier or array access
        else if (arr->current->alias == FL_TOKEN_ALIAS_LBRACKET)
        {
            /// ???????????????????
            // next_token_with_assert;
            // // if next arr->current is RBRACE it has to be dimensions qualifier
            // if (arr->current.alias == FL_TOKEN_ALIAS_RBRACKET)
            // {
            //     (*i)--;
            //     _fl_ast_handle_type_dimensions(node, arr);
            // }
        }
        // it's an simple id or an operator
        else
            _fl_ast_handle_operator(parent, arr, node);

        // check if it is the end of expression
        if (fl_token_is_delim(arr->current))
            fl_ast_node_add_child(parent, node);
        else
            // else it has to be a function call
            _fl_ast_handle_function_call(parent, arr, node);
    }
    else
        throw_syntax_error("Unexpected arr->current");

__return:

    if (inner)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_RPAREN)
        {
            throw_syntax_error("Missing closure of group ')'");
        }
        else
            next_token;
    }

    return parent;
}

// handles single block ({...}) of code and its children
static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token_arr *arr, fl_bool is_root)
{
    // temp variable
    fl_ast_node *node;

    // create parent node (block node)
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, parent);
    parent = node;

    // check for brace if block is not root
    if (!is_root)
    {
        if (arr->current->alias != FL_TOKEN_ALIAS_LBRACE)
        {
            throw_syntax_error("Expected block opening '{'");
        }
        else
            next_token_with_assert;
    }

    // iterate over tokens
    while (arr->current)
    {
        switch (arr->current->type)
        {
        case FL_TOKEN_TYPE_DELIMITER:
            switch (arr->current->alias)
            {
            // inner block to handle
            case FL_TOKEN_ALIAS_LBRACE:
                _fl_ast_handle_block(parent, arr, FL_FALSE);
                break;
            // its end of a block if we meet ending brace
            // and if it is not root
            // (since nested braces will be handled recursively)
            case FL_TOKEN_ALIAS_RBRACE:
                if (is_root)
                {
                    throw_syntax_error("Unexpected block closure ('}')");
                }
                else
                    next_token;
                break;
            case FL_TOKEN_ALIAS_DOT:
                _fl_ast_handle_expression(parent, arr, FL_FALSE);
                break;
            case FL_TOKEN_ALIAS_SEMICOLON:
            case FL_TOKEN_ALIAS_NL:
                next_token;
                break;
            default:
                throw_syntax_error("Unexpected arr->current");
                break;
            }
            break;
        case FL_TOKEN_TYPE_IDENTIFIER:
            _fl_ast_handle_expression(parent, arr, FL_FALSE);
            break;
        case FL_TOKEN_TYPE_LITERAL:
            // to be changed
            // ignore if unused for now
            if (FL_TRUE)
                break;

            // t. b. c.
            switch (arr->current->alias)
            {
            case FL_TOKEN_ALIAS_STRING:
                break;
            case FL_TOKEN_ALIAS_NUMBER:
                break;
            }
            break;
        }
    }
}

static fl_ast_node *_fl_ast_handle_operator(fl_ast_node *parent, fl_token_arr *arr, fl_ast_node *left)
{
    fl_ast_node *node;

    // create operator node
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_OPERATOR, FL_NULL, parent);

    // add id node
    _fl_ast_handle_id_chain(node, arr);
    // add left side of the operator as a child
    fl_ast_node_add_child(node, left);

    if (arr->current->alias == FL_TOKEN_ALIAS_LBRACE)
    {
        // handle block on the right side of the operator...
        _fl_ast_handle_block(node, arr, FL_FALSE);
    }
    else
    {
        // ... else handle expression
        _fl_ast_handle_expression(node, arr, FL_FALSE);
    }

    return node;
}

static fl_void _fl_ast_handle_function_header(fl_ast_node *parent, fl_token_arr *arr)
{
    assert(parent->type == FL_AST_NODE_TYPE_FUNCTION);

    // TODO

    fl_ast_node *statement;
    fl_ast_node *node;
    fl_bool parenth = FL_FALSE;

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_FUNC_HEADER, FL_NULL, parent);
    parent = node;

    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN)
    {
        parenth = FL_TRUE;
        next_token_with_assert;
    }

    do
    {
        // parameter id definition

        if (arr->current->type != FL_TOKEN_TYPE_IDENTIFIER)
        {
            // throw_syntax_error(, arr->current);
        }

        fl_ast_node_init(&statement, FL_AST_NODE_TYPE_STATEMENT, FL_NULL, parent);
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(arr->current->value), statement);

        next_token_with_assert;

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
            next_token_with_assert;
        }
    }
}

static fl_void _fl_ast_handle_function_expression(fl_ast_node *parent, fl_token_arr *arr)
{
    fl_ast_node *func_node;

    fl_ast_node_init(&func_node, FL_AST_NODE_TYPE_FUNCTION, FL_NULL, parent);

    if (arr->current->alias == FL_TOKEN_ALIAS_LPAREN || arr->current->type == FL_TOKEN_TYPE_IDENTIFIER)
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

        fl_ast_node_init(&block_node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, parent);

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
        next_token_with_assert;

        // throw error if bracket is not closed
        if (arr->current->alias != FL_TOKEN_ALIAS_RBRACKET)
            throw_syntax_error("Wrong use of type dimensions specifier");

        next_token_with_assert;
        dimensions++;
    }

    // create type descriptor
    fl_ast_type_info *type_info;
    fl_ast_type_info_init(&type_info, dimensions);
    type_id->data = type_info;
}

// tries to generate abstract syntax tree from specified tokens
fl_bool fl_ast_gen(fl_ast_node **root, fl_token_arr *arr)
{
    fl_token_arr_reset_ptr(arr);

    // create root node with no related data
    fl_ast_node_init(root, FL_AST_NODE_TYPE_ROOT, FL_NULL, FL_NULL);

    // we assume that the whole program is a block
    _fl_ast_handle_block(*root, arr, FL_TRUE);

    return FL_TRUE;
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

fl_void fl_ast_type_info_init(fl_ast_type_info **info, fl_uint dimensions)
{
    fl_ast_type_info *i = fl_mem_alloc(sizeof(fl_ast_type_info));

    i->dimensions = dimensions;

    *info = i;
}

fl_void fl_ast_node_init(fl_ast_node **node, fl_ast_node_type type, fl_unknown *data, fl_ast_node *parent)
{
    fl_ast_node *n = fl_mem_alloc(sizeof(fl_ast_node));

    n->type = type;
    n->data = data;
    n->children = FL_NULL;
    n->next = FL_NULL;

    if (parent)
    {
        fl_ast_node_add_child(parent, n);
    }
    else
    {
        n->parent = FL_NULL;
    }

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

fl_void fl_ast_type_info_free(fl_ast_type_info *info)
{
    fl_mem_free(info);
}

fl_void fl_ast_literal_info_init(fl_ast_literal_info **info, fl_ast_literal_type type, fl_unknown *value)
{
    fl_ast_literal_info *i = fl_mem_alloc(sizeof(fl_ast_literal_info));

    i->literal_type = type;
    i->data = value;

    *info = i;
}

fl_void fl_ast_literal_info_free(fl_ast_literal_info *info)
{
    fl_mem_free(info);
}