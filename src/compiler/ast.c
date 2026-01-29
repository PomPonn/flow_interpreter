#include "compiler/ast.h"

#include "shared/token.h"
#include "shared/memory.h"
#include "shared/error.h"

// --- DEFINES ---

// shorthand for accessing current token
#define token tokens[*i]

// shorthand for increasing token index without check
#define weak_increment ((*i)++ < tok_n)

#define end_of_code (*i >= tok_n)

// shorthand for increasing current token index with error throw if it fails
#define strong_increment \
    if (!weak_increment) \
        fl_err_throw(FL_ERR_AST_SYNTAX, "Unexpected end of code", tokens[*(i - 1)]);

// --- STATIC FUNCTIONS DEFINITION ---

inline static fl_ast_node *_fl_ast_handle_function_call(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_ast_node *func_id);
static fl_void _fl_ast_handle_type_dimensions(fl_ast_node *type_id, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_bool is_root);
static fl_ast_node *_fl_ast_handle_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_function_header(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_function_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_ast_node *_fl_ast_handle_id_chain(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_ast_node *_fl_ast_handle_operator(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_ast_node *left);

// --- IMPLEMENTATIONS ---

// handles id chain (eg. '.body.hand')
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
        strong_increment;
    }

    // iterate over tokens
    while (token.type == FL_TOKEN_TYPE_IDENTIFIER)
    {
        // create id node
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(token.value), parent);

        // check for end of code or chaining dot
        if (!weak_increment || token.alias != FL_TOKEN_ALIAS_DOT)
            break;

        // skip remaining dot (move to next id)
        strong_increment;
    }

    // id chain handler failed
    if (parent->children == FL_NULL)
        fl_err_throw(FL_ERR_AST_SYNTAX, "Expected id chain", token);
    // consume space which separates id chains if it exists
    if (token.alias == FL_TOKEN_ALIAS_SPACE)
        strong_increment;

    return parent;
}

static fl_void _fl_ast_handle_function_header(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    // TODO

    assert(parent->type == FL_AST_NODE_TYPE_FUNCTION);

    fl_ast_node *statement;
    fl_ast_node *node;
    fl_bool parenth = FL_FALSE;

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_FUNC_HEADER, FL_NULL, parent);
    parent = node;

    if (token.alias == FL_TOKEN_ALIAS_LPAREN)
    {
        parenth = FL_TRUE;
        strong_increment;
    }

    do
    {
        // parameter id definition

        if (token.type != FL_TOKEN_TYPE_IDENTIFIER)
        {
            // fl_err_throw(FL_ERR_AST_SYNTAX, token);
        }

        fl_ast_node_init(&statement, FL_AST_NODE_TYPE_STATEMENT, FL_NULL, parent);
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(token.value), statement);

        strong_increment;

        // TODO: to revise
        // if (token.alias == FL_TOKEN_ALIAS_COLON)
        // {
        //     // parameter type definition

        //     strong_increment;
        //     _fl_ast_handle_type_dimensions(statement, tokens, i, tok_n);
        // }
    } while (token.alias == FL_TOKEN_ALIAS_COMMA);

    if (parenth)
    {
        if (token.alias != FL_TOKEN_ALIAS_RPAREN)
        {
            // fl_err_throw(FL_ERR_AST_SYNTAX, token);
        }
        else
        {
            strong_increment;
        }
    }
}

inline static fl_ast_node *_fl_ast_handle_function_call(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n, fl_ast_node *func_id)
{
    fl_ast_node *node;
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_CALL, FL_NULL, parent);
    parent = node;

    // add id as a first child
    fl_ast_node_add_child(parent, func_id);

    // ... handle parameters

    // expect lparen
    if (token.alias != FL_TOKEN_ALIAS_LPAREN)
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, "Expected '(' when calling function with complex argument", token);
    }

    strong_increment;

    do
    {
        _fl_ast_handle_expression(parent, tokens, i, tok_n);
    } while (token.alias == FL_TOKEN_ALIAS_COMMA);

    // expect rparen
    if (token.alias != FL_TOKEN_ALIAS_RPAREN)
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, "Missing ')' (closing of function call)", token);
    }

    weak_increment;

    return parent;
}

// handles any kind of expression
// almost everything is an expression (apart from blocks)
// function may contain a single expression, which is 'instantiated' on call
// it can be a literal, function call, group of operators or a combination of these things
static fl_ast_node *_fl_ast_handle_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    // WIP

    // temp node
    fl_ast_node *node;

    fl_bool parenths = token.alias == FL_TOKEN_ALIAS_LPAREN;
    // expression in parenths
    if (parenths)
    {
        strong_increment;

        node = _fl_ast_handle_expression(FL_NULL, tokens, i, tok_n);

        if (token.alias != FL_TOKEN_ALIAS_RPAREN)
            fl_err_throw(FL_ERR_AST_SYNTAX, "Missing closure of group ')'", token);

        weak_increment;
    }

    if (token.type == FL_TOKEN_TYPE_LITERAL)
    {
        fl_ast_literal_info *literal_info;
        fl_ast_literal_info_init(&literal_info, token.alias, token.value);

        fl_ast_node_init(&node, FL_AST_NODE_TYPE_LITERAL, literal_info, FL_NULL);

        // unexpected error may occure here?

        strong_increment;

        // check if it is the end of expression
        if (!fl_token_is_delim(&token))
        {
            // operator has to be next since it is an literal
            _fl_ast_handle_operator(parent, tokens, i, tok_n, node);
        }
        else
        {
            fl_ast_node_add_child(parent, node);
        }
    }
    // identifier can be either left side of the operator or a function name
    else if (fl_token_is_id_like(&token))
    {
        node = _fl_ast_handle_id_chain(FL_NULL, tokens, i, tok_n);

        if (end_of_code)
        {
            fl_ast_node_add_child(parent, node);
            return parent;
        }

        // if lparen is next, it's definitely a function
        if (token.alias == FL_TOKEN_ALIAS_LPAREN)
        {
            node = _fl_ast_handle_function_call(FL_NULL, tokens, i, tok_n, node);
        }
        // lbrace after id means that we have a dimensions qualifier or array access
        else if (token.alias == FL_TOKEN_ALIAS_LBRACKET)
        {
            strong_increment;
            // if next token is RBRACE it has to be dimensions qualifier
            if (token.alias == FL_TOKEN_ALIAS_RBRACKET)
            {
                (*i)--;
                _fl_ast_handle_type_dimensions(node, tokens, i, tok_n);
            }
        }
        // it's an simple id or an operator
        else
        {
            _fl_ast_handle_operator(parent, tokens, i, tok_n, node);
        }

        // check if it is the end of expression
        if (fl_token_is_delim(&token))
        {
            fl_ast_node_add_child(parent, node);
        }
        else
        {
            // else it has to be a function call
            _fl_ast_handle_function_call(parent, tokens, i, tok_n, node);
        }
    }
    else
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, "Unexpected token", token);
    }
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
            fl_err_throw(FL_ERR_AST_SYNTAX, "Expected block opening '{'", token);
        }
        else
        {
            strong_increment;
        }
    }

    // iterate over tokens
    while (!end_of_code)
    {
        switch (token.type)
        {
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
                if (is_root)
                    fl_err_throw(FL_ERR_AST_SYNTAX, "Unexpected block closure ('}')", token);
                else
                    weak_increment;
                break;
            case FL_TOKEN_ALIAS_DOT:
                _fl_ast_handle_expression(parent, tokens, i, tok_n);
                break;
            case FL_TOKEN_ALIAS_SEMICOLON:
            case FL_TOKEN_ALIAS_NL:
                weak_increment;
                break;
            default:
                fl_err_throw(FL_ERR_AST_SYNTAX, "Unexpected token", token);
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

    // create operator node
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_OPERATOR, FL_NULL, parent);

    // add id node
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

// handles type dimensions
static fl_void _fl_ast_handle_type_dimensions(fl_ast_node *type_id, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    // handle array dimensions
    fl_uint dimensions = 0;
    for (; token.alias == FL_TOKEN_ALIAS_LBRACKET; dimensions++)
    {
        strong_increment;

        // throw error if bracket is not closed
        if (token.alias != FL_TOKEN_ALIAS_RBRACKET)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, "Wrong use of type dimensions specifier", token);
        }

        strong_increment;
        dimensions++;
    }

    // create type descriptor
    fl_ast_type_info *type_info;
    fl_ast_type_info_init(&type_info, dimensions);
    type_id->data = type_info;
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