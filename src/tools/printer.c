#include "printer.h"

#include <stdio.h>

// --- STATIC FUNCTION DEFINITIONS ---

static fl_void _fl_ptr_print_ast_node(fl_ast_node *node);

// --- IMPLEMENTATIONS ---

static fl_void _fl_ptr_print_ast_node(fl_ast_node *node)
{
    printf("> ");
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_ROOT:
        printf("<root>");
        break;
    case FL_AST_NODE_TYPE_BLOCK:
        printf("<block>");
        break;
    case FL_AST_NODE_TYPE_IMPORT:
        printf("<import>");
        break;
    case FL_AST_NODE_TYPE_TYPE:
        printf("<type> [%u]", ((fl_ast_type_node_info *)node->data)->dimensions);
        break;
    case FL_AST_NODE_TYPE_STATEMENT:
        printf("<statement>");
        break;
    case FL_AST_NODE_TYPE_FUNCTION:
        printf("<function>");
        break;
    case FL_AST_NODE_TYPE_FUNC_HEADER:
        printf("<header>");
        break;
    case FL_AST_NODE_TYPE_EXPRESSION:
        printf("<expression>");
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        printf("<literal> '%s'", (fl_char *)node->data);
        break;
    case FL_AST_NODE_TYPE_IDENTIFIER:
        printf("<id> '%s'", (fl_char *)node->data);
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
        printf("<id chain>");
        break;
    case FL_AST_NODE_TYPE_OPERATOR:
        printf("<operator> '%s'", (fl_char *)node->data);
        break;
    default:
        printf("<node>");
        break;
    }
}

fl_void fl_prt_print_token_entries(fl_token *tokens, fl_size n)
{
    printf("<%lu entries>\n", n);

    for (fl_size i = 0; i < n; i++)
    {
        printf("[%lu, %lu] ", tokens[i].line + 1, tokens[i].column + 1);
        printf("'%s'\n", fl_token_str_as_printable(tokens[i].value));
    }
}

fl_void fl_prt_print_tokens_joined(fl_token *tokens, fl_size n)
{
    for (fl_size i = 0; i < n; i++)
    {
        printf("%s", tokens[i].value);
    }
}

fl_void fl_prt_print_ast_tree(fl_ast_node *root, fl_ushort nest_level)
{
    for (fl_ushort i = 0; i < nest_level - 1; i++)
    {
        printf("  ");
    }

    _fl_ptr_print_ast_node(root);

    fl_ast_node *node = root->children;

    if (node)
        printf(":\n");
    else
        printf("\n");

    while (node)
    {
        fl_prt_print_ast_tree(node, nest_level + 1);
        node = node->next;
    }
}
