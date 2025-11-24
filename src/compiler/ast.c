#include "compiler/ast.h"

#include "shared/token.h"
#include "shared/memory.h"
#include "shared/error.h"

// --- DEFINES ---

// --- STATIC FUNCTIONS DEFINITION ---

static void _fl_check_statement_end(fl_token token);
static void _fl_increase_tok_index(fl_size *i, fl_token *tokens, fl_size tok_n);
static fl_size _fl_ast_gen_helper(fl_ast_node *parent, fl_token *tokens, fl_size i, fl_size tokens_num);
static void _fl_ast_handle_use_keyword(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num);
static void _fl_ast_handle_type(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num);
static void _fl_ast_handle_func_header(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num);
static void _fl_ast_handle_statement(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num);
static void _fl_ast_handle_func(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num);
static void _fl_ast_handle_block(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num, fl_bool braced);
static void _fl_ast_handle_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num);

// --- IMPLEMENTATIONS ---

static void _fl_check_statement_end(fl_token token)
{
    if (token.alias != FL_TOKEN_ALIAS_SEMICOLON && token.alias != FL_TOKEN_ALIAS_NL)
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, token);
    }
}

static void _fl_increase_tok_index(fl_size *i, fl_token *tokens, fl_size tok_n)
{
    if (*i + 1 >= tok_n)
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
    }

    (*i)++;
}

static void _fl_ast_handle_func_header(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num)
{
    assert(parent->type == FL_AST_NODE_TYPE_FUNCTION);

    fl_bool braces = FL_FALSE;
    fl_ast_node *header;
    fl_ast_node *statement;
    fl_ast_node *node;

    fl_ast_node_init(&header, FL_AST_NODE_TYPE_FUNC_HEADER, FL_NULL);
    fl_ast_node_add_child(parent, header);

    if (tokens[*i].alias == FL_TOKEN_ALIAS_LBRACE)
    {
        braces = FL_TRUE;
        _fl_increase_tok_index(i, tokens, tokens_num);
    }

    do
    {
        if (tokens[*i].type != FL_TOKEN_TYPE_IDENTIFIER)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }

        fl_ast_node_init(&statement, FL_AST_NODE_TYPE_STATEMENT, FL_NULL);
        fl_ast_node_add_child(header, statement);

        fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(tokens[*i].value));
        fl_ast_node_add_child(statement, node);

        _fl_increase_tok_index(i, tokens, tokens_num);

        if (tokens[*i].alias == FL_TOKEN_ALIAS_COLON)
        {
            _fl_increase_tok_index(i, tokens, tokens_num);
            _fl_ast_handle_type(statement, tokens, i, tokens_num);
        }
    } while (tokens[*i].alias == FL_TOKEN_ALIAS_COMMA);
}

static void _fl_ast_handle_expression(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num)
{
}

static void _fl_ast_handle_block(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num, fl_bool braced)
{
    fl_bool force_postdot = FL_FALSE;
    fl_ast_node *node = FL_NULL;

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_BLOCK, FL_NULL);
    fl_ast_node_add_child(parent, node);

    parent = node;

    if (braced)
    {
        if (tokens[*i].alias != FL_TOKEN_ALIAS_LBRACE)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }
        else
        {
            _fl_increase_tok_index(i, tokens, tokens_num);
        }
    }

    for (; *i < tokens_num; (*i)++)
    {
        if (force_postdot && tokens[*i].type != FL_TOKEN_TYPE_IDENTIFIER && tokens[*i].type != FL_TOKEN_TYPE_KEYWORD)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }

        force_postdot = FL_FALSE;

        switch (tokens[*i].type)
        {
        case FL_TOKEN_TYPE_COMMENT:
            break;
        case FL_TOKEN_TYPE_DELIMITER:
            switch (tokens[*i].alias)
            {
            case FL_TOKEN_ALIAS_DOT:
                force_postdot = FL_TRUE;
                break;
            case FL_TOKEN_ALIAS_RBRACE:
                if (braced)
                    return;
            default:
                break;
            }
            break;
        case FL_TOKEN_TYPE_KEYWORD:
            switch (tokens[*i].alias)
            {
            case FL_TOKEN_ALIAS_USE:
                _fl_increase_tok_index(i, tokens, tokens_num);

                _fl_ast_handle_use_keyword(parent, tokens, i, tokens_num);
                _fl_check_statement_end(tokens[*i]);
                break;
            case FL_TOKEN_ALIAS_EXPORT:
                // Handle export keyword

                break;
            }
            break;
        case FL_TOKEN_TYPE_IDENTIFIER:
            switch (parent->type)
            {
            case FL_AST_NODE_TYPE_BLOCK:
                _fl_ast_handle_statement(parent, tokens, i, tokens_num);
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

static void _fl_ast_handle_func(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num)
{
    fl_ast_node *func_node;

    fl_ast_node_init(&func_node, FL_AST_NODE_TYPE_FUNCTION, FL_NULL);
    fl_ast_node_add_child(parent, func_node);

    if (tokens[*i].alias == FL_TOKEN_ALIAS_LPAREN || tokens[*i].type == FL_TOKEN_TYPE_IDENTIFIER)
    {
        _fl_ast_handle_func_header(func_node, tokens, i, tokens_num);
    }

    if (tokens[*i].alias == FL_TOKEN_ALIAS_LBRACE)
    {
        // function block
        _fl_ast_handle_block(func_node, tokens, i, tokens_num, FL_TRUE);
    }
    else
    {
        // single expression
        fl_ast_node *block_node;

        fl_ast_node_init(&block_node, FL_AST_NODE_TYPE_BLOCK, FL_NULL);
        fl_ast_node_add_child(parent, block_node);

        _fl_ast_handle_expression(block_node, tokens, i, tokens_num);
    }
}

static void _fl_ast_handle_statement(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num)
{
    assert(tokens[*i].type == FL_TOKEN_TYPE_IDENTIFIER);

    fl_ast_node *statement_node;
    fl_ast_node *node;

    fl_ast_node_init(&statement_node, FL_AST_NODE_TYPE_STATEMENT, FL_NULL);
    fl_ast_node_add_child(parent, statement_node);

    fl_ast_node_init(&node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(tokens[*i].value));
    fl_ast_node_add_child(statement_node, node);

    _fl_increase_tok_index(i, tokens, tokens_num);

    if (tokens[*i].alias == FL_TOKEN_ALIAS_COLON)
    {
        _fl_increase_tok_index(i, tokens, tokens_num);
        _fl_ast_handle_type(statement_node, tokens, i, tokens_num);
    }

    switch (tokens[*i].alias)
    {
    case FL_TOKEN_ALIAS_NL:
    case FL_TOKEN_ALIAS_SEMICOLON:
        // WARNING??? - unused
        if (parent->type != FL_AST_NODE_TYPE_BLOCK)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }
        break;
    case FL_TOKEN_ALIAS_ASSIGN:
        // assign expression
        _fl_increase_tok_index(i, tokens, tokens_num);
        _fl_ast_handle_expression(statement_node, tokens, i, tokens_num);
        break;
    case FL_TOKEN_ALIAS_LAMBDA:
        // assign function
        _fl_increase_tok_index(i, tokens, tokens_num);
        _fl_ast_handle_func(statement_node, tokens, i, tokens_num);
        break;
    default:
        // invalid token after declaration
        fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        break;
    }
}

static void _fl_ast_handle_use_keyword(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num)
{
    if (tokens[*i].alias != FL_TOKEN_ALIAS_DOT)
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
    }

    fl_ast_node *import_node;
    fl_ast_node_init(&import_node, FL_AST_NODE_TYPE_IMPORT, FL_NULL);
    fl_ast_node_add_child(parent, import_node);

    while (tokens[*i].alias == FL_TOKEN_ALIAS_DOT)
    {
        (*i)++;

        if (*i >= tokens_num || tokens[*i].type != FL_TOKEN_TYPE_IDENTIFIER)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }

        fl_ast_node *child_node;
        fl_ast_node_init(&child_node, FL_AST_NODE_TYPE_IDENTIFIER, fl_str_dup(tokens[*i].value));
        fl_ast_node_add_child(import_node, child_node);

        (*i)++;
    }
}

static void _fl_ast_handle_type(fl_ast_node *parent, fl_token *tokens, fl_size *i, fl_size tokens_num)
{
    if (tokens[*i].type != FL_TOKEN_TYPE_IDENTIFIER)
    {
        fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
    }

    fl_char *type_id = fl_str_dup(tokens[*i].value);
    fl_uint dimensions = 0;

    _fl_increase_tok_index(i, tokens, tokens_num);

    for (; tokens[*i].alias == FL_TOKEN_ALIAS_LBRACKET; dimensions++)
    {
        _fl_increase_tok_index(i, tokens, tokens_num);
        if (tokens[*i].alias != FL_TOKEN_ALIAS_RBRACKET)
        {
            fl_err_throw(FL_ERR_AST_SYNTAX, tokens[*i]);
        }

        _fl_increase_tok_index(i, tokens, tokens_num);
        dimensions++;
    }

    fl_ast_type_node_info *type_info;
    fl_ast_type_node_info_init(&type_info, type_id, dimensions);

    fl_ast_node *type_node;
    fl_ast_node_init(&type_node, FL_AST_NODE_TYPE_TYPE, type_info);
    fl_ast_node_add_child(parent, type_node);
}

fl_bool fl_ast_gen(fl_ast_node **root, fl_token *tokens, fl_size tokens_num)
{
    if (!root || !*root || !tokens || tokens_num == 0)
        return FL_FALSE;

    fl_ast_node_init(root, FL_AST_NODE_TYPE_ROOT, FL_NULL);

    fl_size iterator = 0;
    _fl_ast_handle_block(*root, tokens, &iterator, tokens_num, FL_FALSE);

    return FL_TRUE;
}

void fl_ast_free(fl_ast_node *node)
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

void fl_ast_type_node_info_init(fl_ast_type_node_info **info, fl_char *type_id, fl_uint dimensions)
{
    fl_ast_type_node_info *i = fl_mem_alloc(sizeof(fl_ast_type_node_info));

    i->type_id = type_id;
    i->dimensions = dimensions;

    *info = i;
}

void fl_ast_node_init(fl_ast_node **node, fl_ast_node_type type, fl_unknown *data)
{
    fl_ast_node *n = fl_mem_alloc(sizeof(fl_ast_node));

    n->type = type;
    n->data = data;
    n->parent = FL_NULL;
    n->children = FL_NULL;
    n->next = FL_NULL;

    *node = n;
}

fl_ast_node *fl_ast_node_peek_child(fl_ast_node *parent)
{
    fl_ast_node *node = parent->children;
    while (node->next)
        node = node->next;
    return node;
}

void fl_ast_node_add_child(fl_ast_node *parent, fl_ast_node *node)
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