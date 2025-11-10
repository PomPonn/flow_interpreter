#include "compiler/ast.h"

#include "shared/token.h"
#include "shared/memory.h"

#include <string.h>

fl_bool fl_ast_gen(fl_ast_node **root, fl_token *tokens, fl_size tokens_num)
{
    if (!root || !*root || !tokens || tokens_num == 0)
        return FL_FALSE;

    fl_ast_node_init(root, FL_AST_NODE_TYPE_ROOT, FL_NULL);

    fl_ast_node *node = *root;

    for (fl_size i = 0; i < tokens_num;)
    {
        switch (tokens[i].type)
        {
        case FL_TOKEN_TYPE_COMMENT:
            i++;
            break;
        case FL_TOKEN_TYPE_KEYWORD:
            switch (tokens[i].alias)
            {
            case FL_TOKEN_ALIAS_IMPORT:
                // Handle import keyword

                if (tokens[i + 1].type == FL_TOKEN_TYPE_IDENTIFIER)
                {
                    i++;

                    // Create an import FL_AST node
                    // fl_ast_node *import_node;
                    // fl_ast_node_init(parent, FL_NULL, FL_NULL, FL_AST_NODE_TYPE_IMPORT);

                    // _cmp_fl_ast_gen_import_children(import_node, tokens, i);
                    // assert(import_node->children != FL_NULL);

                    // ...
                }
                break;
            case FL_TOKEN_ALIAS_EXPORT:
                // Handle export keyword

                // ...
                break;
            }
            break;
        case FL_TOKEN_TYPE_IDENTIFIER:
            // Handle identifier

            switch (node->type)
            {
            case FL_AST_NODE_TYPE_BLOCK:

                break;

            default:
                break;
            }

            break;
        }
    }

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