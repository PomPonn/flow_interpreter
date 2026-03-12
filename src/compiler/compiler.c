#include "compiler.h"

// ---- INCLUDES ----

#include "shared/error.h"
#include "tokenizer/tokenizer.h"
#include "optimalizer/optimalizer.h"
#include "ast/ast_tree.h"
#include "struct/id/id_tree.h"
#include "struct/info/file_location_info.h"
#include "struct/info/operator_info.h"
#include "tools/file.h"
#include "tools/printer.h"

// ---- DEFINES ----

#define _ID_STRING_BUFFER_SIZE 512

#define throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_file_location_info){.description = source_path})

#define throw_compilation_error(ast_node_ptr, message)                             \
    {                                                                              \
        fl_char buffer[_ID_STRING_BUFFER_SIZE];                                    \
        fl_id_chain_to_string(id_chain, buffer, _ID_STRING_BUFFER_SIZE);           \
        fl_err_throw(FL_ERR_COMPILATION, message, buffer, ast_node_ptr->location); \
    }

// ---- DECLARATIONS ----

static fl_void _fl_cmp_compile_node(fl_ast_node *root, fl_context *cnt);
inline static fl_void _fl_cmp_compile_operation(fl_ast_node *op_node, fl_context *cnt, fl_byte_code *dest);
inline static fl_void _fl_cmp_compile_fn_call(fl_ast_node *op_node, fl_context *cnt, fl_byte_code *dest);

// ---- DEFINITIONS ----

// compiles source code to intermediete byte code
fl_void fl_comp_compile(fl_char *source_path, fl_context *cnt)
{
    fl_char *source_code = fl_file_read(source_path);

    if (!source_code)
        throw_file_error("Couldn't read the file (does it exist?)");

    fl_token_arr token_array;
    fl_bool any_token = fl_tok_tokenize(source_code, &token_array, source_path);
    fl_opt_remove_comments(&token_array);

    fl_file_free(source_code);

    if (!any_token)
        // there are no tokens in the file
        return;

    fl_ast_tree tree;
    fl_ast_tree_gen(&tree, &token_array);

    fl_prt_print_ast_tree(&tree);
    return;
    _fl_cmp_compile_node(tree.root, cnt);

    fl_token_arr_free(&token_array);
    fl_ast_tree_free(&tree);
}

static fl_void _fl_cmp_compile_node(fl_ast_node *node, fl_context *cnt)
{
    fl_byte_code bc = cnt->byte_code;

    while (node)
    {
        switch (node->type)
        {
        case FL_AST_NODE_TYPE_BLOCK:
            fl_byte_code_push(&bc, FL_INSTR_STARTBLOCK);
            _fl_cmp_compile_node(node->child, cnt);
            fl_byte_code_push(&bc, FL_INSTR_ENDBLOCK);
            break;
        case FL_AST_NODE_TYPE_CALL:
            _fl_cmp_compile_fn_call(node, cnt, &bc);

            break;
        case FL_AST_NODE_TYPE_OPERATOR:
            _fl_cmp_compile_operation(node, cnt, &bc);
            break;
        default:
            break;
        }

        node = node->next;
    }
}

inline static fl_void _fl_cmp_compile_fn_call(fl_ast_node *op_node, fl_context *cnt, fl_byte_code *dest)
{
    // first child is a function id chain node
    fl_ast_node *id_node = op_node->child;
    fl_id_chain *id_chain = (fl_id_chain *)id_node->data;
    // resolve this function id
    fl_id_entry *id_entry;
    fl_int id_index = fl_context_resolve_id(cnt, id_chain, &id_entry);

    if (!id_entry)
        // throw an error if it's absent
        throw_compilation_error(id_node, "Use of unknown id");

    if (id_entry->type != FL_ID_ENTRY_TYPE_FUNC)
        throw_compilation_error(id_node, "id is not a function");

    // process function arguments first (if any)
    fl_ast_node *arg_node = id_node->next;
    while (arg_node)
    {
        switch (arg_node->type)
        {
        case FL_AST_NODE_TYPE_OPERATOR:
            _fl_cmp_compile_node(op_node, cnt);
            break;
        default:
            assert(FL_FALSE);
            break;
        }

        arg_node = arg_node->next;
    }

    // push id index
    fl_byte_code_push_mem(dest, &id_index, sizeof(id_index));
}

inline static fl_void _fl_cmp_compile_operation(fl_ast_node *op_node, fl_context *cnt, fl_byte_code *dest)
{
    // first child is an operator id
    fl_ast_node *id_node = op_node->child;
    fl_id_chain *id_chain = (fl_id_chain *)id_node->data;
    // resolve this operator id
    fl_id_entry *id_entry;
    fl_int id_index = fl_context_resolve_id(cnt, id_chain, &id_entry);

    if (!id_entry)
        // throw an error if it's absent
        throw_compilation_error(id_node, "Use of unknown id");

    if (id_entry->type != FL_ID_ENTRY_TYPE_OPR)
        throw_compilation_error(id_node, "id is not an operator");

    // get operator args
    fl_ast_node *left_arg = id_node->next;
    fl_ast_node *right_arg = left_arg->next;

    // validate left argument
    if (!left_arg)
        throw_compilation_error(id_node, "Missing left argument of the operator");
    // validate right argument
    if (!right_arg)
        throw_compilation_error(id_node, "Missing right argument of the operator");
    // excess node check
    if (right_arg->next)
        throw_compilation_error(right_arg->next, "Excess operator argument");

    // compile arguments in appropiate order
    if (left_arg->type == FL_AST_NODE_TYPE_OPERATOR && right_arg->type == FL_AST_NODE_TYPE_OPERATOR)
    {
        fl_operator_info *left_info = left_arg->data;
        fl_operator_info *right_info = right_arg->data;
        // reverse processing order if right operator order is higher
        if (right_info->order > left_info->order)
            _fl_swap(fl_ast_node *, right_arg, left_arg);
    }

    _fl_cmp_compile_node(left_arg, cnt);
    _fl_cmp_compile_node(right_arg, cnt);

    // push operator id
    fl_byte_code_push_mem(dest, &id_index, sizeof(id_index));
}