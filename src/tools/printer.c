/**
 * ---------- printer.c -----------------------
 *
 * Declares console printing module logic.
 *
 * --------------------------------------------
 */

#include "printer.h"

// ---------- DEPENDENCIES --------------

#include "shared/str.h"
#include "flow/id/id_chain.h"
#include "flow/value.h"
#include "flow/instr/instruction.h"
#include "tools/byte_reader.h"
#include "macros/basic.h"
#include "flow/section_mark.h"
#include "tokenizer/token_arr.h"
#include "vecs/byte_vec.h"
#include "ast/ast_node.h"

#include <stdio.h>
#include <stdlib.h>

// ---------- MACROS ----------

#define _POSITION_STR_BUFFER_SIZE 16
#define _LITERAL_STR_BUFFER_SIZE 2048
#define _VALUE_STR_BUFFER_SIZE 1024

#define _PRT_HANDLE_PREDEF_PRINT(predef_name_uc)         \
    case _FL_MERGE2(FL_INSTR_, predef_name_uc):          \
        printf("|" _FL_STRINGIFY(predef_name_uc) "|\n"); \
        break

// ---------- DECLARATIONS ----------

inline fl_void _fl_prt_print_ast_node_str(fl_ast_node *node);
inline const fl_char *_fl_prt_convert_ast_literal(fl_ast_node *lit_node);
const fl_char *_fl_prt_value_bytes_to_str(fl_byte_reader *reader);
inline fl_char *_fl_prt_array_to_str(fl_byte_reader *reader);

// ---------- DEFINITIONS ----------

inline const fl_char *_fl_prt_convert_ast_literal(fl_ast_node *lit_node)
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
        assert(sizeof(fl_flow_float) == sizeof(double));
        sprintf(buffer, "%f", *(fl_flow_float *)v->data);
        return buffer;
    case FL_DATA_TYPE_INT:
        assert(sizeof(fl_flow_int) == sizeof(long long));
        sprintf(buffer, "%lld", *(fl_flow_int *)v->data);
        return buffer;
    case FL_DATA_TYPE_BOOL:
    case FL_DATA_TYPE_ARRAY:
    default:
        assert(FL_FALSE);
        return "";
    }
}

inline fl_void _fl_prt_print_ast_node_str(fl_ast_node *node)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        printf("<block>");
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        printf("<literal> %s", _fl_prt_convert_ast_literal(node));
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_char buffer[FL_ID_CHAIN_MAX_SIZE];
        printf(
            "<id chain> '%s'",
            fl_id_chain_to_string(
                (fl_id_chain *)node->data,
                buffer,
                FL_ID_CHAIN_MAX_SIZE));
        break;
    }
    case FL_AST_NODE_TYPE_OPERATOR:
        printf("<operator>");
        break;
    case FL_AST_NODE_TYPE_FN_CALL:
        printf("<call>");
        break;
    case FL_AST_NODE_TYPE_ARRAY:
        printf("<array>");
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
    static fl_char position_str_buffer[_POSITION_STR_BUFFER_SIZE];
    static fl_size nodes_count = 0;

    // printing decorators

    if (root->location.description && (nest_level == 0 || !fl_str_equal(last_desc, root->location.description)))
    {
        last_desc = root->location.description;
        printf("[LOCATION] %s\n", last_desc);
    }

    sprintf(position_str_buffer, "[%u, %u]", root->location.line + 1, root->location.column + 1);
    printf("%-*s", _POSITION_STR_BUFFER_SIZE, position_str_buffer);

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

fl_void fl_prt_print_byte_code_code(fl_byte_reader *reader)
{
    while (!fl_byte_reader_is_end(reader))
    {
        const fl_byte instr = fl_byte_reader_next(reader, fl_byte);
        switch (instr)
        {
        case FL_INSTR_START_BLOCK:
            printf("[BLOCK START]\n");
            break;
        case FL_INSTR_END_BLOCK:
            printf("[BLOCK END]\n");
            break;
        case FL_INSTR_CALL:
            printf("[CALL]\n");
            break;
        case FL_INSTR_GET_ID_BY_INDS:
        {
            assert(sizeof(fl_size) == sizeof(unsigned));

            fl_uint offset = fl_byte_reader_next(reader, fl_uint);
            fl_size ind_count = fl_byte_reader_next(reader, fl_size);
            printf("[ID] %u; ", offset);

            for (fl_size i = 0; i < ind_count; i++)
            {
                fl_int ind = fl_byte_reader_next(reader, fl_int);
                printf("%d%s", ind, i == ind_count - 1 ? "\n" : ", ");
            }

            break;
        }
        case FL_INSTR_GET_ID_BY_NAMES:
            printf("[ID] %s\n", fl_byte_reader_next_str(reader));
            break;
        case FL_INSTR_END_ARGS:
            printf("[ARGS END]\n");
            break;
        case FL_INSTR_NEW_ID:
        {
            const fl_char *id_name = fl_byte_reader_next_str(reader);
            printf("[DECL] '%s'\n", id_name);
            break;
        }
        case FL_INSTR_GET_LITERAL:
        {
            assert(sizeof(fl_size) == sizeof(unsigned));

            fl_size ind = fl_byte_reader_next(reader, fl_size);
            printf("[LITERAL] %u\n", ind);
            break;
        }
        case FL_INSTR_IMMED_VALUE:
            printf("[VALUE] %s\n", _fl_prt_value_bytes_to_str(reader));
            break;
            // ---- PREDEFINED INSTRUCTIONS ----
            _PRT_HANDLE_PREDEF_PRINT(IMPORT);
            _PRT_HANDLE_PREDEF_PRINT(ASSIGN);
            _PRT_HANDLE_PREDEF_PRINT(LAMBDA);
            _PRT_HANDLE_PREDEF_PRINT(ADD);
            _PRT_HANDLE_PREDEF_PRINT(SUB);
            _PRT_HANDLE_PREDEF_PRINT(DIV);
            _PRT_HANDLE_PREDEF_PRINT(MUL);
            _PRT_HANDLE_PREDEF_PRINT(MOD);
            _PRT_HANDLE_PREDEF_PRINT(POW);
        default:
            // undefined instruction
            assert(FL_FALSE);
            break;
        }
    }
}

fl_void fl_prt_print_byte_code_file(const fl_byte_vec bytes)
{
    fl_byte_reader reader = fl_byte_reader_create(bytes);

    if (fl_byte_reader_next(&reader, fl_section_mark) != FL_SECTION_MARK_HEADER)
        assert(FL_FALSE);

    printf("==== HEADER ====\n");

    printf("[BYTE CODE VERSION] %s\n", fl_byte_reader_next_str(&reader));
    printf("[COMPILATION TIMESTAMP] %lld \n", fl_byte_reader_next(&reader, fl_long));

    if (fl_byte_reader_next(&reader, fl_section_mark) == FL_SECTION_MARK_LITERALS)
        printf("==== LITERALS ====\n%s", _fl_prt_array_to_str(&reader));

    if (fl_byte_reader_next(&reader, fl_section_mark) != FL_SECTION_MARK_CODE)
        assert(FL_FALSE);

    printf("==== CODE ====\n");
    fl_prt_print_byte_code_code(&reader);
}

inline fl_char *_fl_prt_array_to_str(fl_byte_reader *reader)
{
    static fl_char buffer[_VALUE_STR_BUFFER_SIZE];
    const fl_size count = fl_byte_reader_next(reader, fl_size);

    sprintf(buffer, "[");

    for (fl_size i = 0; !fl_byte_reader_is_end(reader) && i < count; i++)
    {
        fl_size buf_len = fl_str_len(buffer);
        const fl_char *vstr = _fl_prt_value_bytes_to_str(reader);
        sprintf(buffer + buf_len, i == count - 1 ? "%s]" : "%s, ", vstr);
    }

    return buffer;
}

const fl_char *_fl_prt_value_bytes_to_str(fl_byte_reader *reader)
{
    static fl_char buffer[_VALUE_STR_BUFFER_SIZE];
    const fl_data_type data_type = fl_byte_reader_next(reader, fl_data_type);

    switch (data_type)
    {
    case FL_DATA_TYPE_NULL:
        sprintf(buffer, "NULL");
        break;
    case FL_DATA_TYPE_STRING:
        sprintf(buffer, "'%s'", fl_byte_reader_next_str(reader));
        break;
    case FL_DATA_TYPE_INT:
        assert(sizeof(fl_flow_int) == sizeof(long long));
        sprintf(buffer, "%lld", fl_byte_reader_next(reader, fl_flow_int));
        break;
    case FL_DATA_TYPE_FLOAT:
        assert(sizeof(fl_flow_float) == sizeof(double));
        sprintf(buffer, "%f", fl_byte_reader_next(reader, fl_flow_float));
        break;
    case FL_DATA_TYPE_ARRAY:
        sprintf(buffer, _fl_prt_array_to_str(reader));
        break;
    case FL_DATA_TYPE_BOOL:
    case FL_DATA_TYPE_FN:
    case FL_DATA_TYPE_REF:
    case FL_DATA_TYPE_SCOPE:
    default:
        assert(FL_FALSE);
        break;
    }

    return buffer;
}
