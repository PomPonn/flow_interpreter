/**
 * ---- compiler.c ----
 *
 * Implements compiler logic.
 *
 */

#include "compiler.h"

// ---- DEPENDENCIES ----

#include "shared/error.h"
#include "tokenizer/tokenizer.h"
#include "optimalizer/optimalizer.h"
#include "ast/ast_tree.h"
#include "struct/id/id_node.h"
#include "struct/id/callable_id_entry.h"
#include "struct/info/all.h"
#include "tools/file.h"
#ifndef NDEBUG
#include "tools/printer.h"
#endif

// ---- DEFINES ----

/**
 * @brief Size of id chain string buffer.
 */
#define _ID_STRING_BUFFER_SIZE 512

/**
 * @brief Shorthand for throwing a file error.
 */
#define throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_file_location_info){.description = source_path})

/**
 * @brief Shorthand for throwing a compilation error.
 */
#define throw_compilation_error(ast_node_ptr, message)                             \
    {                                                                              \
        fl_char buffer[_ID_STRING_BUFFER_SIZE];                                    \
        fl_id_chain_to_string(id_chain, buffer, _ID_STRING_BUFFER_SIZE);           \
        fl_err_throw(FL_ERR_COMPILATION, message, buffer, ast_node_ptr->location); \
    }

// ---- DECLARATIONS ----

// TODO: comment these declarations

static fl_void _fl_cmp_compile_node(fl_ast_node *root, fl_context *cnt);
inline static fl_bool _fl_cmp_is_constant(fl_ast_node *node, fl_context *cnt);
inline static fl_void _fl_cmp_compile_opr_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest);
inline static fl_void _fl_cmp_compile_fn_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest);
inline static fl_void _fl_cmp_compile_callable(fl_ast_node *cll_node, fl_context *cnt, fl_byte_vec *dest);

// ---- DEFINITIONS ----

fl_void fl_cmp_compile(fl_char *source_path, fl_context *cnt)
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

    fl_ast_node *tree = fl_ast_tree_gen(&token_arr);

#ifndef NDEBUG
    fl_prt_print_ast_tree(tree);
#endif

    _fl_cmp_compile_node(tree, cnt);

    fl_token_arr_free(&token_arr);
    fl_ast_node_free(tree);
}

inline static fl_bool _fl_cmp_is_constant(fl_ast_node *node, fl_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_LITERAL:
        return FL_TRUE;
    case FL_AST_NODE_TYPE_OPERATOR:
    case FL_AST_NODE_TYPE_FN_CALL:
    {
        // first child is a callable id chain node
        fl_ast_node *node = node->child;
        fl_id_chain *id_chain = (fl_id_chain *)node->data;
        // resolve this callable id
        fl_id_node *id_node;
        fl_context_resolve_id(cnt, FL_NULL, id_chain, &id_node);
        fl_callable_id_entry *callable_id = (fl_callable_id_entry *)id_node->value;

        return callable_id->constant;
    }
    default:
        assert(FL_FALSE);
        break;
    }
}

static fl_void _fl_cmp_compile_node(fl_ast_node *node, fl_context *cnt)
{
    fl_byte_vec *bc = &cnt->byte_vec;

    while (node)
    {
        switch (node->type)
        {
        case FL_AST_NODE_TYPE_BLOCK:
            fl_byte_vec_push(bc, FL_INSTR_STARTBLOCK);
            _fl_cmp_compile_node(node->child, cnt);
            fl_byte_vec_push(bc, FL_INSTR_ENDBLOCK);
            break;
        case FL_AST_NODE_TYPE_FN_CALL:
        case FL_AST_NODE_TYPE_OPERATOR:
            _fl_cmp_compile_callable(node, cnt, bc);
            break;
        default:
            break;
        }

        node = node->next;
    }
}

inline static fl_void _fl_cmp_compile_callable(fl_ast_node *cll_node, fl_context *cnt, fl_byte_vec *dest)
{
    // first child is a callable id chain node
    fl_ast_node *node = cll_node->child;
    fl_id_chain *id_chain = (fl_id_chain *)node->data;
    // resolve this callable id
    fl_id_node *id_node;
    fl_int id_index = fl_context_resolve_id(cnt, FL_NULL, id_chain, (const fl_id_node **)&id_node);
    fl_callable_id_entry *callable_id = (fl_callable_id_entry *)id_node->value;

    if (!id_node)
        // throw an error if id is absent
        throw_compilation_error(node, "Use of unknown callable id");

    if (id_node->value->type != FL_ID_ENTRY_TYPE_CALLABLE)
        throw_compilation_error(node, "id is not callable");

    // process callable arguments
    switch (cll_node->type)
    {
    case FL_AST_NODE_TYPE_FN_CALL:
        node = node->next;
        while (node)
        {
            switch (node->type)
            {
            case FL_AST_NODE_TYPE_OPERATOR:
                // _fl_cmp_compile_node(arg_node, cnt);
                // break;
            default:
                assert(FL_FALSE);
                break;
            }

            node = node->next;
        }
        break;
    case FL_AST_NODE_TYPE_OPERATOR:
        // get operator args
        fl_ast_node *left_arg = node->next;

        // validate left argument
        if (!left_arg)
            throw_compilation_error(cll_node->child, "Missing left argument of the operator");

        fl_ast_node *right_arg = left_arg->next;

        // validate right argument
        if (!right_arg)
            throw_compilation_error(cll_node->child, "Missing right argument of the operator");

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
        break;

        break;
    }

    // push id index
    fl_byte_vec_push_mem(dest, &id_index, sizeof(id_index));
}

inline static fl_void _fl_cmp_compile_fn_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest)
{
}

inline static fl_void _fl_cmp_compile_opr_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest)
{

    // _fl_cmp_compile_node(left_arg, cnt);
    // _fl_cmp_compile_node(right_arg, cnt);
}