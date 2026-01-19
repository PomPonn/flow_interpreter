#include "compiler/ast.h"

#include "shared/token.h"
#include "shared/memory.h"
#include "shared/error.h"

// --- DEFINES ---

// --- STATIC FUNCTIONS DEFINITION ---

static fl_void _fl_ast_increase_tok_index(fl_size *i, fl_token *tokens, fl_size tok_n);
static fl_void _fl_ast_handle_type(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_func_header(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_statement(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_func(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);
static fl_void _fl_ast_handle_id_chain(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n);

// --- IMPLEMENTATIONS ---

static fl_void _fl_ast_increase_tok_index(fl_size *i, fl_token *tokens, fl_size tok_n)
{
    if (*i + 1 >= tok_n)
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
    }

    (*i)++;
}

static fl_void _fl_ast_handle_id_chain(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    fl_ast_node *node;

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_ID_CHAIN, FL_NULL, parent);
    parent = node;

    if (tokens[*i].alias == FL_TOKEN_ALIAS_DOT)
    {
        // add global context as access identifier
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(tokens[*i].value), parent);
        _fl_ast_increase_tok_index(i, tokens, tok_n);
    }

    do
    {
        if (tokens[*i].type != FL_TOKEN_TYPE_IDENTIFIER)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }

        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(tokens[*i].value), parent);

        _fl_ast_increase_tok_index(i, tokens, tok_n);

        if (tokens[*i].alias != FL_TOKEN_ALIAS_DOT)
        {
            break;
        }

        _fl_ast_increase_tok_index(i, tokens, tok_n);
    } while (FL_TRUE);
}

static fl_void _fl_ast_handle_func_header(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    assert(parent->type == FL_AST_NODE_TYPE_FUNCTION);

    fl_ast_node *statement;
    fl_ast_node *node;
    fl_bool parenth = FL_FALSE;

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_FUNC_HEADER, FL_NULL, parent);
    parent = node;

    if (tokens[*i].alias == FL_TOKEN_ALIAS_LPAREN)
    {
        parenth = FL_TRUE;
        _fl_ast_increase_tok_index(i, tokens, tok_n);
    }

    do
    {
        // parameter id definition

        if (tokens[*i].type != FL_TOKEN_TYPE_IDENTIFIER)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }

        fl_ast_node_init(&statement, FL_AST_NODE_TYPE_STATEMENT, FL_NULL, parent);
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(tokens[*i].value), statement);

        _fl_ast_increase_tok_index(i, tokens, tok_n);

        if (tokens[*i].alias == FL_TOKEN_ALIAS_COLON)
        {
            // parameter type definition

            _fl_ast_increase_tok_index(i, tokens, tok_n);
            _fl_ast_handle_type(statement, tokens, i, tok_n);
        }
    } while (tokens[*i].alias == FL_TOKEN_ALIAS_COMMA);

    if (parenth)
    {
        if (tokens[*i].alias != FL_TOKEN_ALIAS_RPAREN)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }
        else
        {
            _fl_ast_increase_tok_index(i, tokens, tok_n);
        }
    }
}

static fl_void _fl_ast_handle_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
}

static fl_void _fl_ast_handle_block(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    fl_ast_node *node = FL_NULL;
    // tODO: do smoething with tyhis
    fl_bool braced = FL_FALSE;

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, parent);

    parent = node;

    if (tokens[*i].alias == FL_TOKEN_ALIAS_LBRACE)
    {
        _fl_ast_increase_tok_index(i, tokens, tok_n);
        braced = FL_TRUE;
    }

    for (; *i < tok_n; (*i)++)
    {
        switch (tokens[*i].type)
        {
        case FL_TOKEN_TYPE_COMMENT:
            break;
        case FL_TOKEN_TYPE_DELIMITER:
            switch (tokens[*i].alias)
            {
            case FL_TOKEN_ALIAS_RBRACE:
                if (braced)
                    return;
            default:
                break;
            }
            break;
        case FL_TOKEN_TYPE_IDENTIFIER:
            switch (parent->type)
            {
            case FL_AST_NODE_TYPE_BLOCK:
                _fl_ast_handle_statement(parent, tokens, i, tok_n);
                break;
            }
            break;
        case FL_TOKEN_TYPE_LITERAL:
            // to change
            // ignore if unused
            if (FL_TRUE)
            {
                break;
            }

            switch (tokens[*i].alias)
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

static fl_void _fl_ast_handle_func(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    fl_ast_node *func_node;

    fl_ast_node_init(&func_node, FL_AST_NODE_TYPE_FUNCTION, FL_NULL, parent);

    if (tokens[*i].alias == FL_TOKEN_ALIAS_LPAREN || tokens[*i].type == FL_TOKEN_TYPE_IDENTIFIER)
    {
        _fl_ast_handle_func_header(func_node, tokens, i, tok_n);
    }

    if (tokens[*i].alias == FL_TOKEN_ALIAS_LBRACE)
    {
        // function block
        _fl_ast_handle_block(func_node, tokens, i, tok_n);
    }
    else
    {
        // single expression
        fl_ast_node *block_node;

        fl_ast_node_init(&block_node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, parent);

        _fl_ast_handle_expression(block_node, tokens, i, tok_n);
    }
}

static fl_void _fl_ast_handle_statement(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    fl_ast_node *statement_node;
    fl_ast_node *node;

    fl_ast_node_init(&statement_node, FL_AST_NODE_TYPE_STATEMENT, FL_NULL, parent);
    _fl_ast_handle_id_chain(statement_node, tokens, i, tok_n);

    if (tokens[*i].alias == FL_TOKEN_ALIAS_COLON)
    {
        _fl_ast_increase_tok_index(i, tokens, tok_n);
        _fl_ast_handle_type(statement_node, tokens, i, tok_n);
    }

    switch (tokens[*i].alias)
    {
    case FL_TOKEN_ALIAS_NL:
    case FL_TOKEN_ALIAS_SEMICOLON:
        // WARNING??? - unused id used

        // if (parent->type != FL_AST_NODE_TYPE_BLOCK)
        // {
        //     fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        // }
        break;
    case FL_TOKEN_ALIAS_DOT:
        fl_ast_node_init(&node, FL_AST_NODE_TYPE_OPERATOR, FL_NULL, statement_node);
        _fl_ast_handle_id_chain(node, tokens, i, tok_n);
        break;
    default:
        // invalid token after declaration
        fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        break;
    }
}

static fl_void _fl_ast_handle_type(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tok_n)
{
    fl_ast_node *type_node;
    fl_ast_node_init(&type_node, FL_AST_NODE_TYPE_TYPE, FL_NULL, parent);

    _fl_ast_handle_id_chain(type_node, tokens, i, tok_n);

    fl_uint dimensions = 0;
    for (; tokens[*i].alias == FL_TOKEN_ALIAS_LBRACKET; dimensions++)
    {
        _fl_ast_increase_tok_index(i, tokens, tok_n);
        if (tokens[*i].alias != FL_TOKEN_ALIAS_RBRACKET)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }

        _fl_ast_increase_tok_index(i, tokens, tok_n);
        dimensions++;
    }

    fl_ast_type_node_info *type_info;
    fl_ast_type_node_info_init(&type_info, dimensions);
    type_node->data = type_info;
}

fl_bool fl_ast_gen(fl_ast_node **root, fl_token *tokens, fl_size tok_n)
{
    if (!root || !*root || !tokens || tok_n == 0)
        return FL_FALSE;

    fl_ast_node_init(root, FL_AST_NODE_TYPE_ROOT, FL_NULL, FL_NULL);

    fl_size iterator = 0;
    _fl_ast_handle_block(*root, tokens, &iterator, tok_n);

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

    // Free current node
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