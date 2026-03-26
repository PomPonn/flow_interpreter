/**
 * ---- compiler.c ----
 *
 * Implements compiler logic.
 *
 */

#include "compiler.h"

// ---- DEPENDENCIES ----

#include "shared/error.h"
#include "shared/str.h"
#include "tokenizer/tokenizer.h"
#include "optimalizer/optimalizer.h"
#include "ast/ast_tree.h"
#include "struct/id/id_node.h"
#include "struct/id/fn_id_data.h"
#include "vecs/arg_vec.h"
#include "lang/scope.h"
#include "lang/value_desc.h"
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
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_token_location){.description = source_path})

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

// TODO: comment these declarations when finished

static fl_value_desc _fl_cmp_compile_node(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static inline fl_void _fl_cmp_compile_block(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static inline fl_void _fl_cmp_compile_fn(
    fl_ast_node *fn_node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *codes);

static inline fl_value_desc _fl_cmp_compile_plain_id_chain(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);
// revise - |
//          v
static inline fl_bool _fl_cmp_is_constant(fl_ast_node *node, fl_context *cnt);
static inline fl_void _fl_cmp_compile_opr_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest);
static inline fl_void _fl_cmp_compile_fn_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest);

// ---- DEFINITIONS ----

fl_void fl_cmp_compile(fl_char *source_path, fl_context *cnt)
{
    fl_char *source_code = fl_file_read(source_path);

    if (!source_code)
        throw_file_error("Couldn't read the file (does it exist?)");

    fl_token_arr token_arr;
    fl_bool any_token = fl_tok_tokenize(source_code, &token_arr, source_path);
    fl_opt_remove_comments(&token_arr);

    fl_file_free(&source_code);

    if (!any_token)
        // there are no tokens in the file
        return;

    fl_ast_node *tree = fl_ast_tree_gen(&token_arr);

#ifndef NDEBUG
    fl_prt_print_ast_tree(tree);
#endif

    fl_byte_vec code;
    fl_byte_vec_init(&code, 0);

    _fl_cmp_compile_node(tree, cnt, &cnt->main_scope, &code);

    fl_token_arr_free(&token_arr);
    fl_ast_node_free(&tree);
}

static fl_value_desc _fl_cmp_compile_node(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        _fl_cmp_compile_block(node, cnt, scope, code);
        return fl_value_desc_create(FL_DATA_TYPE_SCOPE, FL_FALSE);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
        _fl_cmp_compile_fn(node, cnt, scope, code);
        return fl_value_desc_create(FL_DATA_TYPE_FN, FL_FALSE);
    case FL_AST_NODE_TYPE_ID_CHAIN:
        return _fl_cmp_compile_plain_id_chain(node, cnt, scope, code);
    default:
        assert(FL_FALSE);
        return fl_value_desc_create(FL_DATA_TYPE_NULL, FL_FALSE);
        break;
    }
}

static inline fl_void _fl_cmp_compile_block(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_scope inner_scope;
    fl_scope_init(&inner_scope, scope);

    node = node->child;
    while (node)
    {
        _fl_cmp_compile_node(node, cnt, &inner_scope, code);
        node = node->next;
    }
}

static inline fl_void _fl_cmp_compile_fn(
    fl_ast_node *fn_node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_ast_node *id_node = fn_node->child;
    fl_id_chain *id_chain = (fl_id_chain *)id_node->data;

    fl_uint offset;
    fl_int_vec inds = fl_scope_resolve_id(scope, id_chain, &offset);
    fl_id_entry *id = fl_scope_traverse(scope, inds, offset);

    if (!id)
        // throw an error if id does not exist
        throw_compilation_error(id_node, "Use of unknown fn id");

    if (id->data_type != FL_DATA_TYPE_FN)
        // throw an error if id is not fn
        throw_compilation_error(id_node, "id is not fn");

    fl_fn_id_data *data = id->data;

    // compile arguments to deduce function variant
    fl_ast_node *arg_node = id_node->next;
    while (arg_node)
    {
        fl_byte_vec arg_code;
        fl_byte_vec_init(&arg_code, 0);

        fl_value_desc td = _fl_cmp_compile_node(arg_node, cnt, scope, &arg_code);

        arg_node = arg_node->next;
    }

    // if (data->constant)
    // {
    //     if (data->c_defined)
    //     {
    //         fl_c_fn_ptr fn_ptr = data->pointer;

    //         fl_arg_vec args;
    //         fl_arg_vec_init(&args, data->params.size + 1);
    //         // pass context as a first arg
    //         args.data[0] = cnt;
    //         // pass actual args
    //         // copy data?
    //         args.data[1] = node->next->data;

    //         fl_byte_vec *res_code = fn_ptr(args);
    //         // push resulting code (expand this fn call in compile time)
    //         fl_byte_vec_push_mem(code, res_code->data, res_code->size);
    //         // cleanup
    //         fl_byte_vec_free(res_code);
    //         fl_arg_vec_free(&args);
    //         fl_int_vec_free(&inds);
    //     }
    //     return;
    // }

    // // start CALL instruction
    // fl_byte_vec_push(code, FL_INSTR_CALL);
    // // push information how to access id
    // // push scope back offset
    // fl_byte_vec_push_mem(code, &offset, sizeof(offset));
    // // push size of indexes
    // fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
    // // push actual indexes
    // fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);

    // fl_int_vec_free(&inds);

    // // fl_id_node_vec_find(&scope->ids, id_node);
    // // fl_int id_index = fl_context_resolve_id(cnt, FL_NULL, id_chain, (const fl_id_node **)&id_node);
    // // fl_fn_id_data *fn_id = (fl_fn_id_data *)id_node->value;

    // // if (!id_node)
    // //     // throw an error if id is absent
    // //     throw_compilation_error(node, "Use of unknown fn id");

    // // if (!fl_id_entry_fn(id_node->value))
    // //     throw_compilation_error(node, "id is not fn");

    // // process fn arguments
    // switch (fn_node->type)
    // {
    // case FL_AST_NODE_TYPE_FN_CALL:
    //     node = node->next;
    //     while (node)
    //     {
    //         switch (node->type)
    //         {
    //         case FL_AST_NODE_TYPE_OPERATOR:
    //             // _fl_cmp_compile_node(arg_node, cnt);
    //             // break;
    //         default:
    //             assert(FL_FALSE);
    //             break;
    //         }

    //         node = node->next;
    //     }
    //     break;
    // case FL_AST_NODE_TYPE_OPERATOR:
    //     // get operator args
    //     fl_ast_node *left_arg = node->next;

    //     // validate left argument
    //     if (!left_arg)
    //         throw_compilation_error(fn_node->child, "Missing left argument of the operator");

    //     fl_ast_node *right_arg = left_arg->next;

    //     // validate right argument
    //     if (!right_arg)
    //         throw_compilation_error(fn_node->child, "Missing right argument of the operator");

    //     // excess node check
    //     if (right_arg->next)
    //         throw_compilation_error(right_arg->next, "Excess operator argument");

    //     // compile arguments in appropiate order
    //     if (left_arg->type == FL_AST_NODE_TYPE_OPERATOR && right_arg->type == FL_AST_NODE_TYPE_OPERATOR)
    //     {
    //         fl_operator_info *left_info = left_arg->data;
    //         fl_operator_info *right_info = right_arg->data;
    //         // reverse processing order if right operator order is higher
    //         if (right_info->order > left_info->order)
    //             _fl_swap(fl_ast_node *, right_arg, left_arg);
    //     }

    //     fl_value_desc td = _fl_cmp_compile_node(left_arg, cnt, scope, code);
    //     // check if argument type descriptors are fine
    //     if (!fl_value_desc_match(data->params.data[0], td))
    //         throw_compilation_error(left_arg, "Argument cannot be used in this context");

    //     td = _fl_cmp_compile_node(right_arg, cnt, scope, code);
    //     // same with second argument
    //     if (!fl_value_desc_match(data->params.data[1], td))
    //         throw_compilation_error(left_arg, "Argument cannot be used in this context");

    //     break;
    // default:
    //     assert(FL_FALSE);
    //     break;
    // }
}

static inline fl_value_desc _fl_cmp_compile_plain_id_chain(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_id_chain *id_chain = (fl_id_chain *)node->data;

    fl_uint offset;
    fl_int_vec inds = fl_scope_resolve_id(scope, id_chain, &offset);
    fl_id_entry *id = fl_scope_traverse(scope, inds, offset);

    if (id)
    {
        // push information to access id
        fl_byte_vec_push(code, FL_INSTR_GET);
        // push scope back offset
        fl_byte_vec_push_mem(code, &offset, sizeof(offset));
        // push size of indexes
        fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
        // push actual indexes
        fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);

        fl_int_vec_free(&inds);
        return fl_value_desc_create(id->data_type, FL_TRUE);
    }

    if (id_chain->size == 1)
    {
        // push information to create new id
        fl_byte_vec_push(code, FL_INSTR_NEW);

        // create dummy id in this scope
        fl_id_entry *entry;
        fl_id_entry_init(&entry, fl_str_dup(id_chain->data[0]), FL_DATA_TYPE_NULL, FL_NULL);
        fl_id_entry_vec_push(&scope->ids, entry);
    }

    fl_int_vec_free(&inds);
    return fl_value_desc_create(FL_DATA_TYPE_NULL, FL_TRUE);
}

static inline fl_void _fl_cmp_compile_fn_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest)
{
}

static inline fl_void _fl_cmp_compile_opr_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest)
{

    // _fl_cmp_compile_node(left_arg, cnt);
    // _fl_cmp_compile_node(right_arg, cnt);
}