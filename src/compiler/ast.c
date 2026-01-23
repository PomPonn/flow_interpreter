#include "compiler/ast.h"

#include "shared/token.h"
#include "shared/memory.h"
#include "shared/error.h"

#include <assert.h>

// --- DEFINES ---

// shorthand for accessing current token
#define token tokens[*i]
#define increment _fl_ast_increase_tok_index(i, tokens, tok_n)

// --- STATIC FUNCTIONS DEFINITION ---

static fl_void _fl_ast_increase_tok_index(fl_size *i, fl_token *tokens, fl_size tok_n);
static fl_void _fl_ast_handle_type(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_statement(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_bool is_root);
static fl_void _fl_ast_handle_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_function_header(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_function_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_ast_node *_fl_ast_handle_function_call(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_ast_node *func_id);
static fl_ast_node *_fl_ast_handle_id_chain(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_ast_node *_fl_ast_handle_operator(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_ast_node *left);

// --- IMPLEMENTATIONS ---

static fl_void _fl_ast_increase_tok_index(fl_size *i, fl_token *tokens, fl_size tok_n)
{
    if ((*i)++ >= tok_n)
        fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*(i - 1)]);
}

// id chain (.body.hand) expression
static fl_ast_node *_fl_ast_handle_id_chain(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    fl_ast_node *node;

    // init parent (id chain) node
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_ID_CHAIN, FL_NULL, parent);
    parent = node;

    // dot at the beginning means access to global context
    // so we mark it as a flag
    if (token.alias == FL_TOKEN_ALIAS_DOT)
    {
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_FLAG, FL_NULL, parent);
        increment;
    }

    // iterate over tokens
    while (token.type == FL_TOKEN_TYPE_IDENTIFIER)
    {
        // create id node
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(token.value), parent);

        increment;

        // check for chaining dot
        if (token.alias != FL_TOKEN_ALIAS_DOT)
            break;

        increment;
    }

    // id chain handler failed
    if (parent->children == FL_NULL)
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, token);
    }

    return parent;
}

static fl_void _fl_ast_handle_function_header(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    assert(parent->type == FL_AST_NODE_TYPE_FUNCTION);

    fl_ast_node *statement;
    fl_ast_node *node;
    fl_bool parenth = FL_FALSE;

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_FUNC_HEADER, FL_NULL, parent);
    parent = node;

    if (token.alias == FL_TOKEN_ALIAS_LPAREN)
    {
        parenth = FL_TRUE;
        increment;
    }

    do
    {
        // parameter id definition

        if (token.type != FL_TOKEN_TYPE_IDENTIFIER)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, token);
        }

        fl_ast_node_init(&statement, FL_AST_NODE_TYPE_STATEMENT, FL_NULL, parent);
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(token.value), statement);

        increment;

        // TODO: to revise
        // if (token.alias == FL_TOKEN_ALIAS_COLON)
        // {
        //     // parameter type definition

        //     increment;
        //     _fl_ast_handle_type(statement, tokens, i, tok_n);
        // }
    } while (token.alias == FL_TOKEN_ALIAS_COMMA);

    if (parenth)
    {
        if (token.alias != FL_TOKEN_ALIAS_RPAREN)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, token);
        }
        else
        {
            increment;
        }
    }
}

static fl_ast_node *_fl_ast_handle_function_call(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_ast_node *func_id)
{
    fl_ast_node *node;

    // ...

    return node;
}

// handles any kind of expression
// almost everything is an expression (apart from blocks)
// function may contain a single expression, which is 'instantiated' on call
// it can be a literal, function call, group of operators or a combination of these things
static fl_void _fl_ast_handle_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    // temp node
    fl_ast_node *node;

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_EXPRESSION, FL_NULL, parent);
    parent = node;

    while (*i < tok_n)
    {
        // if literal is first, we expect operator next
        if (token.type == FL_TOKEN_TYPE_LITERAL)
        {
            fl_ast_literal_node_info *literal_info;
            fl_ast_literal_node_info_init(&literal_info, token.alias);

            fl_ast_node_init(&node, FL_AST_NODE_TYPE_LITERAL, literal_info, parent);

            increment;

            _fl_ast_handle_operator(parent, tokens, i, tok_n, node);
        }
        // identifier can be either left side of the operator or a function name
        else if (fl_token_is_id_like(&token))
        {
            node = _fl_ast_handle_id_chain(FL_NULL, tokens, i, tok_n);

            // if lparen is next, it's definitely a function
            if (token.alias == FL_TOKEN_ALIAS_LPAREN)
            {
                node = _fl_ast_handle_function_call(FL_NULL, tokens, i, tok_n, node);
            }

            // check if it is the end of expression
            if (fl_token_is_expr_killer(&token))
            {
                increment;
                return;
            }

            // if it's not, there has to be operator
            _fl_ast_handle_operator(parent, tokens, i, tok_n, node);
        }
    }

    // // check for id chain on beginning
    // _fl_ast_handle_id_chain(parent, tokens, i, tok_n);

    // // check if type is being assigned
    // if (token.alias == FL_TOKEN_ALIAS_COLON)
    // {
    //     increment;
    //     // handle type setting
    //     _fl_ast_handle_type(parent, tokens, i, tok_n);
    // }

    // // check if it's a traditional function call
    // // (calls without parens are also valid)
    // if (token.alias == FL_TOKEN_ALIAS_LPAREN)
    // {
    // }
    // // check for some id next
    // else if (token.type != FL_TOKEN_TYPE_IDENTIFIER && token.alias != FL_TOKEN_ALIAS_DOT)
    // {
    //     // error if it's absent
    //     // (it could be unused variable declaration, but let's ignore it for now)
    //     fl_err_throw(FL_ERR_AST_SYNTAX, token);
    // }

    // _fl_ast_handle_id_chain(parent, tokens, i, tok_n);

    // // there is another argument
    // if (token.alias == FL_TOKEN_ALIAS_COMMA)
    // {
    // }

    // _fl_ast_handle_operator(parent, tokens, i, tok_n);

    // // check for some kind of 'operator' next (at this compilation stage it is interpreted as indentifier)
    // if (token.type != FL_TOKEN_TYPE_IDENTIFIER && token.alias != FL_TOKEN_ALIAS_DOT)
    // {
    //     // error if it's absent
    //     fl_err_throw(FL_ERR_AST_SYNTAX, token);
    // }

    // // create operator node and process it's id
    // fl_ast_node_init(&node, FL_AST_NODE_TYPE_OPERATOR, FL_NULL, parent);
    // _fl_ast_handle_id_chain(node, tokens, i, tok_n);

    // if (token.alias == FL_TOKEN_ALIAS_LBRACE)
    // {
    //     // handle block on the right side of the operator...
    //     _fl_ast_handle_block(parent, tokens, i, tok_n, FL_FALSE);
    // }
    // else
    // {
    //     // ... else handle expression
    //     _fl_ast_handle_expression(parent, tokens, i, tok_n);
    // }
}

// handles single block ({...}) of code and its children
static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_bool is_root)
{
    // temp variable
    fl_ast_node *node = FL_NULL;
    // TODO: do smoething with tyhis

    // create parent node (block node)
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, parent);
    parent = node;

    // check for brace if block is not root
    if (!is_root)
    {
        if (token.alias != FL_TOKEN_ALIAS_LBRACE)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, token);
        }
        else
        {
            increment;
        }
    }

    // iterate over tokens
    while (*i < tok_n)
    {
        switch (token.type)
        {
        // we ignore comments, if they were present for some reason
        // (they should be removed before AST generation process)
        case FL_TOKEN_TYPE_COMMENT:
            break;
        case FL_TOKEN_TYPE_DELIMITER:
            switch (token.alias)
            {
            // inner block to handle
            case FL_TOKEN_ALIAS_LBRACE:
                _fl_ast_handle_block(parent, tokens, i, tok_n, FL_FALSE);
                break;
            // its end of a block if we meet ending brace
            // and if it is not root
            // (since nested braces will be handled recursively)
            case FL_TOKEN_ALIAS_RBRACE:
                if (!is_root)
                    continue;
                else
                    fl_err_throw(FL_ERR_AST_SYNTAX, token);
            case FL_TOKEN_ALIAS_DOT:
                _fl_ast_handle_expression(parent, tokens, i, tok_n);
                break;
            default:
                break;
            }
            break;
        case FL_TOKEN_TYPE_IDENTIFIER:
            _fl_ast_handle_expression(parent, tokens, i, tok_n);
            break;
        case FL_TOKEN_TYPE_LITERAL:
            // to be changed
            // ignore if unused for now
            if (FL_TRUE)
            {
                break;
            }

            // t. b. c.
            switch (token.alias)
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

static fl_void _fl_ast_handle_function_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    fl_ast_node *func_node;

    fl_ast_node_init(&func_node, FL_AST_NODE_TYPE_FUNCTION, FL_NULL, parent);

    if (token.alias == FL_TOKEN_ALIAS_LPAREN || token.type == FL_TOKEN_TYPE_IDENTIFIER)
    {
        _fl_ast_handle_function_header(func_node, tokens, i, tok_n);
    }

    if (token.alias == FL_TOKEN_ALIAS_LBRACE)
    {
        // function block
        _fl_ast_handle_block(func_node, tokens, i, tok_n, FL_FALSE);
    }
    else
    {
        // single expression
        fl_ast_node *block_node;

        fl_ast_node_init(&block_node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, parent);

        _fl_ast_handle_expression(block_node, tokens, i, tok_n);
    }
}

static fl_ast_node *_fl_ast_handle_operator(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_ast_node *left)
{
    fl_ast_node *node;

    // check for some kind of 'operator' next (at this compilation stage it is interpreted as indentifier)
    if (token.type != FL_TOKEN_TYPE_IDENTIFIER && token.alias != FL_TOKEN_ALIAS_DOT)
    {
        // error if it's absent
        fl_err_throw(FL_ERR_AST_SYNTAX, token);
    }

    // create operator node
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_OPERATOR, FL_NULL, parent);
    // process operator id
    _fl_ast_handle_id_chain(node, tokens, i, tok_n);
    // add left side of the operator as a child
    fl_ast_node_add_child(node, left);

    if (token.alias == FL_TOKEN_ALIAS_LBRACE)
    {
        // handle block on the right side of the operator...
        _fl_ast_handle_block(node, tokens, i, tok_n, FL_FALSE);
    }
    else
    {
        // ... else handle expression
        _fl_ast_handle_expression(node, tokens, i, tok_n);
    }

    return node;
}

static fl_void _fl_ast_handle_statement(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    // fl_ast_node *node;

    // // init parent (statement) node
    // fl_ast_node_init(&node, FL_AST_NODE_TYPE_STATEMENT, FL_NULL, parent);
    // parent = node;

    // // check for id chain on beginning
    // _fl_ast_handle_id_chain(parent, tokens, i, tok_n);

    // // check if type is being assigned
    // if (token.alias == FL_TOKEN_ALIAS_COLON)
    // {
    //     increment;
    //     // handle type setting
    //     _fl_ast_handle_type(parent, tokens, i, tok_n);
    // }

    // // check if it's a traditional function call
    // // (calls without parens are also valid)
    // if (token.alias == FL_TOKEN_ALIAS_LPAREN)
    // {
    // }
    // // check for some id next
    // else if (token.type != FL_TOKEN_TYPE_IDENTIFIER && token.alias != FL_TOKEN_ALIAS_DOT)
    // {
    //     // error if it's absent
    //     // (it could be unused variable declaration, but let's ignore it for now)
    //     fl_err_throw(FL_ERR_AST_SYNTAX, token);
    // }

    // _fl_ast_handle_id_chain(parent, tokens, i, tok_n);

    // // there is another argument
    // if (token.alias == FL_TOKEN_ALIAS_COMMA)
    // {
    // }

    // _fl_ast_handle_operator(parent, tokens, i, tok_n);

    // // check for some kind of 'operator' next (at this compilation stage it is interpreted as indentifier)
    // if (token.type != FL_TOKEN_TYPE_IDENTIFIER && token.alias != FL_TOKEN_ALIAS_DOT)
    // {
    //     // error if it's absent
    //     fl_err_throw(FL_ERR_AST_SYNTAX, token);
    // }

    // // create operator node and process it's id
    // fl_ast_node_init(&node, FL_AST_NODE_TYPE_OPERATOR, FL_NULL, parent);
    // _fl_ast_handle_id_chain(node, tokens, i, tok_n);

    // if (token.alias == FL_TOKEN_ALIAS_LBRACE)
    // {
    //     // handle block on the right side of the operator...
    //     _fl_ast_handle_block(parent, tokens, i, tok_n, FL_FALSE);
    // }
    // else
    // {
    //     // ... else handle expression
    //     _fl_ast_handle_expression(parent, tokens, i, tok_n);
    // }
}

// handles type expression (part after colon in variable declaration)
static fl_void _fl_ast_handle_type(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    fl_ast_node *type_node;
    fl_ast_node_init(&type_node, FL_AST_NODE_TYPE_TYPE, FL_NULL, parent);

    // check for id chain, which can resolve to type
    _fl_ast_handle_id_chain(type_node, tokens, i, tok_n);

    // handle array dimensions (if any)
    fl_uint dimensions = 0;
    for (; token.alias == FL_TOKEN_ALIAS_LBRACKET; dimensions++)
    {
        increment;

        // throw error if bracket is not closed
        if (token.alias != FL_TOKEN_ALIAS_RBRACKET)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, token);
        }

        increment;
        dimensions++;
    }

    // create type descriptor
    fl_ast_type_node_info *type_info;
    fl_ast_type_node_info_init(&type_info, dimensions);
    type_node->data = type_info;
}

// tries to generate abstract syntax tree from specified tokens
fl_bool fl_ast_gen(fl_ast_node **root, fl_token *tokens, fl_size tok_n)
{
    assert(root && *root && tokens && tok_n != 0);

    // create root node with no related data
    fl_ast_node_init(root, FL_AST_NODE_TYPE_ROOT, FL_NULL, FL_NULL);

    fl_size iterator = 0;
    // we assume that the whole program is a block
    _fl_ast_handle_block(*root, tokens, &iterator, tok_n, FL_TRUE);

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

fl_void fl_ast_type_node_info_init(fl_ast_type_node_info **info, fl_uint dimensions)
{
    fl_ast_type_node_info *i = fl_mem_alloc(sizeof(fl_ast_type_node_info));

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

fl_void fl_ast_type_node_info_free(fl_ast_type_node_info *info)
{
    fl_mem_free(info);
}

fl_void fl_ast_literal_node_info_init(fl_ast_literal_node_info **info, fl_ast_literal_type type)
{
    fl_ast_literal_node_info *i = fl_mem_alloc(sizeof(fl_ast_literal_node_info));

    i->literal_type = type;

    *info = i;
}

fl_void fl_ast_literal_node_info_free(fl_ast_literal_node_info *info)
{
    fl_mem_free(info);
}