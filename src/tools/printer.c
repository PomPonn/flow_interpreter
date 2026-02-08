#include "printer.h"

#include <stdio.h>

// --- STATIC FUNCTION DEFINITIONS ---

static inline fl_void _fl_prt_print_ast_node(fl_ast_node *node);
static inline fl_char *_fl_prt_convert_literal(fl_ast_node *literal_node);

// --- IMPLEMENTATIONS ---

static inline fl_char *_fl_prt_convert_literal(fl_ast_node *literal_node)
{
    assert(literal_node->type == FL_AST_NODE_TYPE_LITERAL);

    // temp
    return (fl_char *)((fl_ast_literal_info *)literal_node->data)->data;
}

static inline fl_void _fl_prt_print_ast_node(fl_ast_node *node)
{
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
        printf("<type> [%u]", ((fl_ast_type_info *)node->data)->dimensions);
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
        printf("<literal> '%s'", _fl_prt_convert_literal(node));
        break;
    case FL_AST_NODE_TYPE_IDENTIFIER:
        printf("<id> '%s'", (fl_char *)node->data);
        break;
    case FL_AST_NODE_TYPE_ID:
        printf("<id chain>");
        break;
    case FL_AST_NODE_TYPE_OPERATOR:
        printf("<operator>");
        break;
    case FL_AST_NODE_TYPE_CALL:
        printf("<call>");
        break;
    default:
        printf("<node>");
        break;
    }
}

fl_void fl_prt_print_token_entries(fl_token_arr *arr)
{
    printf("<%lu entries>\n", arr->_count);

    while (arr->current)
    {
        printf("[%lu, %lu] ", arr->current->line + 1, arr->current->column + 1);
        printf("'%s'\n", fl_token_str_as_printable(arr->current->value));
        fl_token_arr_next(arr);
    }
}

fl_void fl_prt_print_tokens_joined(fl_token_arr *arr)
{
    while (arr->current)
    {
        printf("%s", arr->current->value);
        fl_token_arr_next(arr);
    }
}

fl_void fl_prt_print_ast_tree(fl_ast_node *root, fl_ushort nest_level)
{
    for (fl_ushort i = 0; i < nest_level; i++)
    {
        printf((i == nest_level - 1) ? "+- " : "|  ");
    }

    _fl_prt_print_ast_node(root);

    fl_ast_node *node = root->children;

    printf(node ? ":\n" : "\n");

    while (node)
    {
        fl_prt_print_ast_tree(node, nest_level + 1);
        node = node->next;
    }
}
