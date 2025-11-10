#include "compiler.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

static inline void _cmp_ast_gen_import_children(ast_node *import_node,
                                                inp_token *tokens,
                                                size_t *index)
{
    assert(import_node != NULL);
    assert(tokens != NULL);
    assert(index != NULL);

    if (tokens[*index].alias == COMP_TOKEN_ALIAS_DOT)
    {
        // add global import flag
        ast_node *global_flag_node;
        ast_node_init(&global_flag_node, AST_NODE_TYPE_FLAG, _strdup(tokens[*index].value));
        ast_node_add_child(import_node, global_flag_node);
        *index++;
    }

    do
    {
        if (tokens[*index].type == COMP_TOKEN_TYPE_IDENTIFIER)
        {
            ast_node *identifier_node;
            ast_node_init(&identifier_node, AST_NODE_TYPE_IDENTIFIER, _strdup(tokens[*index].value));
            ast_node_add_child(import_node, identifier_node);
            *index++;
        }
        else
        {
            // syntax error
            assert(0);
        }

    } while (tokens[*index++].type != COMP_TOKEN_TYPE_DELIMITER);
}

static inline void _cmp_ast_gen_subtree(ast_node *parent, inp_token *tokens,
                                        size_t *index,
                                        const size_t tokens_num)
{
    assert(parent != NULL);
    assert(tokens != NULL);
    assert(index != NULL);

    if (*index >= tokens_num)
        return;

    switch (tokens[*index].type)
    {
    case COMP_TOKEN_TYPE_COMMENT:
        *index++;
        break;
    case COMP_TOKEN_TYPE_KEYWORD:
        switch (tokens[*index].alias)
        {
        case COMP_TOKEN_ALIAS_IMPORT:
            // Handle import keyword

            if (tokens[*index + 1].type == COMP_TOKEN_TYPE_IDENTIFIER)
            {
                index++;

                // Create an import AST node
                ast_node *import_node;
                // ast_node_init(parent, NULL, NULL, AST_NODE_TYPE_IMPORT);

                _cmp_ast_gen_import_children(import_node, tokens, index);
                assert(import_node->children != NULL);

                // ...
            }
            break;
        case COMP_TOKEN_ALIAS_EXPORT:
            // Handle export keyword

            // ...
            break;
        }
        break;
    case COMP_TOKEN_TYPE_IDENTIFIER:
        // Handle identifier

        switch (parent->type)
        {
        case AST_NODE_TYPE_BLOCK:

            break;

        default:
            break;
        }

        break;
    }

    // Recursively generate subtrees
    _cmp_ast_gen_subtree(parent, tokens, index, tokens_num);
}

void cmp_compile_to_intermediate(inp_token *tokens, size_t tokens_num)
{
}