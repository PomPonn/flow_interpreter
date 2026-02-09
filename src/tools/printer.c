#include "printer.h"

#include <stdio.h>

// --- DEFINES ---

#define _LOCATION_STR_BUFFER_SIZE 16

// --- STATIC FUNCTION DEFINITIONS ---

static inline fl_void _fl_prt_print_ast_node(fl_ast_node *node);
static inline fl_char *_fl_prt_convert_literal(fl_ast_node *literal_node);

// --- IMPLEMENTATIONS ---

static inline fl_char *_fl_prt_convert_literal(fl_ast_node *literal_node)
{
    assert(literal_node->type == FL_AST_NODE_TYPE_LITERAL);

    // temp
    return (fl_char *)((fl_ast_literal_data *)literal_node->data)->data;
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
    case FL_AST_NODE_TYPE_TYPE:
        printf("<type> [%u]", ((fl_ast_type_data *)node->data)->dimensions);
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
    case FL_AST_NODE_TYPE_ID_CHAIN:
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
        printf("[%lu, %lu] ", arr->current->location.line + 1, arr->current->location.column + 1);
        printf("'%s'\n", fl_token_as_printable(arr->current->value));
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
    static const fl_char *last_desc = FL_NULL;
    static fl_char location_str_buffer[_LOCATION_STR_BUFFER_SIZE];
    static fl_size nodes_count = 0;

    // printing decorators

    if (nest_level == 0 || !fl_str_equal(last_desc, root->location.description))
    {
        last_desc = root->location.description;
        printf("[LOCATION] %s\n", last_desc);
    }

    sprintf(location_str_buffer, "[%lu, %lu]", root->location.line + 1, root->location.column + 1);
    printf("%-*s", _LOCATION_STR_BUFFER_SIZE, location_str_buffer);

    for (fl_ushort i = 0; i < nest_level; i++)
    {
        printf((i == nest_level - 1) ? "+- " : "|  ");
    }

    // printing current node

    _fl_prt_print_ast_node(root);
    fl_ast_node *node = root->children;
    printf(node ? ":\n" : "\n");

    nodes_count++;

    // printing all children
    while (node)
    {
        fl_prt_print_ast_tree(node, nest_level + 1);
        node = node->next;
    }

    // printing node count
    if (nest_level == 0)
    {
        printf("[NODES COUNT] %lu", nodes_count);
        nodes_count = 0;
    }
}
