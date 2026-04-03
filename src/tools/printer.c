#include "printer.h"

#include "shared/str.h"
#include "lang/id_chain.h"
#include "lang/value.h"
#include <stdio.h>
#include <stdlib.h>

// ---------- MACROS ----------

#define _LOCATION_STR_BUFFER_SIZE 16
#define _ID_CHAIN_STR_BUFFER_SIZE 256
#define _LITERAL_STR_BUFFER_SIZE 2048

// ---------- DECLARATIONS ----------

static inline fl_void _fl_prt_print_ast_node_str(fl_ast_node *node);
static inline const fl_char *_fl_prt_convert_ast_literal(fl_ast_node *lit_node);

// ---------- DEFINITIONS ----------

static inline const fl_char *_fl_prt_convert_ast_literal(fl_ast_node *lit_node)
{
    static fl_char buffer[_LITERAL_STR_BUFFER_SIZE];

    assert(lit_node->type == FL_AST_NODE_TYPE_LITERAL);

    fl_value *v = lit_node->data;

    switch (v->data_type)
    {
    case FL_DATA_TYPE_STRING:
        sprintf(buffer, "'%s'", (fl_char *)v->data);
        return buffer;
    case FL_DATA_TYPE_NULL:
        return "NULL";
    case FL_DATA_TYPE_FLOAT:
        assert(sizeof(fl_flow_type_float) == sizeof(double));
        sprintf(buffer, "%f", *(fl_flow_type_float *)v->data);
        return buffer;
    case FL_DATA_TYPE_INT:
        assert(sizeof(fl_flow_type_int) == sizeof(long long));
        sprintf(buffer, "%lld", *(fl_flow_type_int *)v->data);
        return buffer;
    case FL_DATA_TYPE_BOOL:
    case FL_DATA_TYPE_ARRAY:
    default:
        assert(FL_FALSE);
        return "";
    }
}

static inline fl_void _fl_prt_print_ast_node_str(fl_ast_node *node)
{

    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        printf("<block>");
        break;
    case FL_AST_NODE_TYPE_FUNCTION:
        printf("<function>");
        break;
    case FL_AST_NODE_TYPE_FUNC_HEADER:
        printf("<header>");
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        printf("<ast_literal> %s", _fl_prt_convert_ast_literal(node));
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_char buffer[_ID_CHAIN_STR_BUFFER_SIZE];
        printf(
            "<id chain> '%s'",
            fl_id_chain_to_string(
                (fl_id_chain *)node->data,
                buffer,
                _ID_CHAIN_STR_BUFFER_SIZE));
        break;
    }
    case FL_AST_NODE_TYPE_OPERATOR:
        printf("<operator>");
        break;
    case FL_AST_NODE_TYPE_FN_CALL:
        printf("<call>");
        break;
    default:
        printf("<node>");
        break;
    }
}

fl_void fl_prt_print_token_entries(fl_token_arr *arr)
{
    printf("<%u entries>\n", arr->_count);

    while (arr->current)
    {
        printf("[%u, %u] ", arr->current->location.line + 1, arr->current->location.column + 1);
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

fl_void fl_prt_print_ast_node(fl_ast_node *root, fl_ushort nest_level)
{
    static const fl_char *last_desc = FL_NULL;
    static fl_char location_str_buffer[_LOCATION_STR_BUFFER_SIZE];
    static fl_size nodes_count = 0;

    // printing decorators

    if (root->location.description && (nest_level == 0 || !fl_str_equal(last_desc, root->location.description)))
    {
        last_desc = root->location.description;
        printf("[LOCATION] %s\n", last_desc);
    }

    sprintf(location_str_buffer, "[%u, %u]", root->location.line + 1, root->location.column + 1);
    printf("%-*s", _LOCATION_STR_BUFFER_SIZE, location_str_buffer);

    for (fl_ushort i = 0; i < nest_level; i++)
    {
        printf((i == nest_level - 1) ? "+- " : "|  ");
    }

    // printing current node

    _fl_prt_print_ast_node_str(root);
    fl_ast_node *node = root->child;
    printf(node ? ":\n" : "\n");

    nodes_count++;

    // printing all children
    while (node)
    {
        fl_prt_print_ast_node(node, nest_level + 1);
        node = node->next;
    }

    // printing node count
    if (nest_level == 0)
    {
        printf("[NODES COUNT] %u\n", nodes_count);
        nodes_count = 0;
    }
}
