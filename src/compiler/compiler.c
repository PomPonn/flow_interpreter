#include "compiler.h"

// ---- INCLUDES ----

#include "shared/error.h"
#include "tokenizer/tokenizer.h"
#include "optimalizer/optimalizer.h"
#include "ast/ast_tree.h"
#include "struct/id/id_node.h"
#include "struct/info/location_info.h"
#include "struct/info/operator_info.h"
#include "tools/file.h"
#include "tools/printer.h"

// ---- DEFINES ----

#define _ID_STRING_BUFFER_SIZE 512

#define throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_location_info){.description = source_path})

#define throw_compilation_error(ast_node_ptr, message)                             \
    {                                                                              \
        fl_char buffer[_ID_STRING_BUFFER_SIZE];                                    \
        fl_id_chain_to_string(id_chain, buffer, _ID_STRING_BUFFER_SIZE);           \
        fl_err_throw(FL_ERR_COMPILATION, message, buffer, ast_node_ptr->location); \
    }

// ---- DECLARATIONS ----

static fl_void _fl_cmp_compile_node(fl_ast_node *root, fl_context *cnt);
inline static fl_void _fl_cmp_compile_opr_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_code *dest);
inline static fl_void _fl_cmp_compile_fn_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_code *dest);
inline static fl_void _fl_cmp_compile_callable(fl_ast_node *cll_node, fl_context *cnt, fl_byte_code *dest);

// ---- DEFINITIONS ----

// compiles source code to intermediete byte code
fl_void fl_comp_compile(fl_char *source_path, fl_context *cnt)
{
    fl_char *source_code = fl_file_read(source_path);

    if (!source_code)
        throw_file_error("Couldn't read the file (does it exist?)");

    fl_token_arr token_arr;
    fl_bool any_token = fl_tok_tokenize(source_code, &token_arr, source_path);
    fl_opt_remove_comments(&token_arr);

    fl_file_free(source_code);

    if (!any_token)
        // there are no tokens in the file
        return;

    fl_ast_tree tree;
    fl_ast_tree_gen(&tree, &token_arr);

    fl_prt_print_ast_tree(&tree);

    _fl_cmp_compile_node(tree.root, cnt);

    fl_token_arr_free(&token_arr);
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
        case FL_AST_NODE_TYPE_FN_CALL:
        case FL_AST_NODE_TYPE_OPERATOR:
            _fl_cmp_compile_callable(node, cnt, &bc);
            break;
        default:
            break;
        }

        node = node->next;
    }
}

inline static fl_void _fl_cmp_compile_callable(fl_ast_node *cll_node, fl_context *cnt, fl_byte_code *dest)
{
    // first child is a callable id chain node
    fl_ast_node *node = cll_node->child;
    fl_id_chain *id_chain = (fl_id_chain *)node->data;
    // resolve this callable id
    fl_id_node *id_node;
    fl_int id_index = fl_context_resolve_id(cnt, id_chain, (const fl_id_node **)&id_node);

    if (!id_node)
        // throw an error if id is absent
        throw_compilation_error(node, "Use of unknown callable id");

    if (id_node->value->type != FL_ID_ENTRY_TYPE_CALLABLE)
        throw_compilation_error(node, "id is not callable");

    // process callable arguments
    switch (cll_node->type)
    {
    case FL_AST_NODE_TYPE_FN_CALL:
        _fl_cmp_compile_fn_args(node->next, id_chain, cnt, dest);
        break;
    case FL_AST_NODE_TYPE_OPERATOR:
        _fl_cmp_compile_opr_args(node->next, id_chain, cnt, dest);
        break;
    default:
        assert(FL_FALSE);
        break;
    }

    // push id index
    fl_byte_code_push_mem(dest, &id_index, sizeof(id_index));
}

inline static fl_void _fl_cmp_compile_fn_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_code *dest)
{
    while (arg_node)
    {
        switch (arg_node->type)
        {
        case FL_AST_NODE_TYPE_OPERATOR:
            _fl_cmp_compile_node(arg_node, cnt);
            break;
        default:
            assert(FL_FALSE);
            break;
        }

        arg_node = arg_node->next;
    }
}

inline static fl_void _fl_cmp_compile_opr_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_code *dest)
{
    // get operator args
    fl_ast_node *left_arg = arg_node;

    // validate left argument
    if (!left_arg)
        throw_compilation_error(arg_node->parent->child, "Missing left argument of the operator");

    fl_ast_node *right_arg = left_arg->next;

    // validate right argument
    if (!right_arg)
        throw_compilation_error(arg_node->parent->child, "Missing right argument of the operator");

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
}