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
#include "revise_me/id/id_node.h"
#include "vecs/fn_variant_vec.h"
#include "lang/scope.h"
#include "lang/value_desc.h"
#include "lang/value.h"
#include "lang/instructions.h"
#include "tools/file.h"
#ifndef NDEBUG
#include "tools/printer.h"
#endif

// ---- MACROS ----

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

static fl_void _fl_cmp_compile_node(
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

static inline fl_void _fl_cmp_compile_plain_id_chain(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static inline fl_void _fl_cmp_compile_literal(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static inline fl_value_desc _fl_cmp_deduce_value_desc(
    const fl_ast_node *node,
    const fl_scope *scope);

// try to reduce given ast node to some constant value
static inline fl_bool _fl_cmp_const_reduce(
    const fl_ast_node *node,
    fl_context *cnt,
    const fl_scope *scope,
    fl_value *reduced_value);

// reduced args have to be initialized
static inline fl_bool _fl_cmp_const_reduce_fn_args(
    const fl_ast_node *arg_node,
    fl_context *cnt,
    const fl_scope *scope,
    fl_value_vec *reduced_args);

static inline fl_fn_variant *_fl_cmp_infer_fn_overload(
    const fl_ast_node *arg_node,
    const fl_scope *scope,
    const fl_fn_variant_vec *variants);

// revise - |
//          v
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
    fl_byte_vec_init(&code);

    _fl_cmp_compile_node(tree, cnt, &cnt->main_scope, &code);

    fl_token_arr_free(&token_arr);
    fl_ast_node_free(&tree);
}

static fl_void _fl_cmp_compile_node(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        _fl_cmp_compile_block(node, cnt, scope, code);
        break;
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
        _fl_cmp_compile_fn(node, cnt, scope, code);
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
        _fl_cmp_compile_plain_id_chain(node, cnt, scope, code);
        break;
    case FL_AST_NODE_TYPE_LITERAL:

        break;
    default:
        assert(FL_FALSE);
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
    fl_int_vec inds = fl_scope_find_id_path(scope, id_chain, &offset);
    fl_id_entry *id = fl_scope_traverse(scope, inds, offset);

    if (!id)
        // throw an error if id does not exist
        throw_compilation_error(id_node, "Use of unknown function id");

    if (id->value.data_type != FL_DATA_TYPE_FN)
        // throw an error if id is not fn
        throw_compilation_error(id_node, "id is not a function");

    // deduce function argument types
    fl_fn_variant *fn_var = _fl_cmp_infer_fn_overload(id_node->next, scope, id->value.data);

    // TODO: better error message needed
    if (!fn_var)
        throw_compilation_error(id_node, "couldn't find function overload for given arguments");

    if (fn_var->constant)
    {
        // run this function immidiately and inline it

        // resolve arguments
        fl_value_vec args;
        fl_value_vec_initc(&args, fn_var->params.size);

        if (!_fl_cmp_const_reduce_fn_args(id_node->next, cnt, scope, &args))
        {
            fl_value_vec_free(&args);
            assert(FL_FALSE);
        }

        fl_value res = fl_fn_variant_call(fn_var, args);
        fl_value_vec_free(&args);
    }
    else
    {
        // compile this function call to byte code

        fl_byte_vec args_code;
        fl_byte_vec_init(&args_code);

        fl_ast_node *arg_node = id_node->next;
        while (arg_node)
        {
            _fl_cmp_compile_node(arg_node, cnt, scope, &args_code);
            arg_node = arg_node->next;
        }

        // start CALL instruction
        fl_byte_vec_push(code, FL_INSTR_CALL);
        // push information how to access id
        // push scope back offset
        fl_byte_vec_push_mem(code, &offset, sizeof(offset));
        // push size of indexes
        fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
        // push actual indexes
        fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);

        // push args code
        fl_byte_vec_push_mem(code, args_code.data, args_code.size);

        fl_byte_vec_free(&args_code);
    }
}

static inline fl_void _fl_cmp_compile_literal(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_value *value = node->data;
    fl_uint index = fl_context_make_literal(cnt, *value);

    fl_byte_vec_push(code, FL_INSTR_GET_LITERAL);
    fl_byte_vec_push_mem(code, &index, sizeof(index));
}

static inline fl_void _fl_cmp_compile_plain_id_chain(
    fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_id_chain *id_chain = (fl_id_chain *)node->data;

    fl_uint offset;
    fl_int_vec inds = fl_scope_find_id_path(scope, id_chain, &offset);
    fl_id_entry *id = fl_scope_traverse(scope, inds, offset);

    if (id)
    {
        // push information to access id
        fl_byte_vec_push(code, FL_INSTR_GET_ID);
        // push scope back offset
        fl_byte_vec_push_mem(code, &offset, sizeof(offset));
        // push size of indexes
        fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
        // push actual indexes
        fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);
    }
    else if (id_chain->size == 1)
    {
        // push information to create new id
        fl_byte_vec_push(code, FL_INSTR_NEW);

        // create dummy id in this scope
        fl_id_entry *entry;
        fl_id_entry_init(&entry, fl_str_dup(id_chain->data[0]), FL_DATA_TYPE_NULL, FL_VALUE_EMPTY);
        fl_id_entry_vec_push(&scope->ids, entry);
    }
    else
    {
        // error: undefined id
        assert(FL_FALSE);
    }

    fl_int_vec_free(&inds);
}

static inline fl_void _fl_cmp_compile_fn_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest)
{
}

static inline fl_void _fl_cmp_compile_opr_args(fl_ast_node *arg_node, fl_id_chain *id_chain, fl_context *cnt, fl_byte_vec *dest)
{

    // _fl_cmp_compile_node(left_arg, cnt);
    // _fl_cmp_compile_node(right_arg, cnt);
}

static inline fl_value_desc _fl_cmp_deduce_value_desc(
    const fl_ast_node *node,
    const fl_scope *scope)
{
    fl_id_entry *id = FL_NULL;

    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        return fl_value_desc_create(FL_DATA_TYPE_SCOPE, FL_FALSE);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
        id = fl_scope_resolve_id(scope, node->child->data);
        return fl_value_desc_create(id->value.data_type, FL_FALSE);
    case FL_AST_NODE_TYPE_ID_CHAIN:
        id = fl_scope_resolve_id(scope, node->data);
        if (id)
            return fl_value_desc_create(id->value.data_type, FL_TRUE);
        else
            return fl_value_desc_create(FL_DATA_TYPE_NULL, FL_TRUE);
    case FL_AST_NODE_TYPE_LITERAL:
        return fl_value_desc_create(((fl_value *)node->data)->data_type, FL_FALSE);
    default:
        assert(FL_FALSE);
        return fl_value_desc_create(FL_DATA_TYPE_NULL, FL_FALSE);
    }
}

static inline fl_bool _fl_cmp_const_reduce(
    const fl_ast_node *node,
    fl_context *cnt,
    const fl_scope *scope,
    fl_value *reduced_value)
{
    fl_id_entry *id = FL_NULL;

    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
    {
        fl_scope *inner = fl_mem_alloc(sizeof(fl_scope));
        fl_scope_init(inner, scope);

        // ???

        // fl_ast_node *scope_node = node->child;
        // while (scope_node)
        // {
        //     fl_value reduced;
        //     if (!_fl_cmp_const_reduce(scope_node, cnt, &inner, &reduced))
        //     {
        //         fl_scope_free(scope_node);
        //         return FL_FALSE;
        //     }
        // }

        *reduced_value = fl_value_create(FL_DATA_TYPE_SCOPE, inner);
        break;
    }
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
        id = fl_scope_resolve_id(scope, node->child->data);

        if (id->value.data_type != FL_DATA_TYPE_FN)
            return FL_FALSE;

        fl_fn_variant *fn = _fl_cmp_infer_fn_overload(node->child->next, scope, id->value.data);

        if (!fn || !fn->constant)
            return FL_FALSE;

        fl_value_vec args;
        fl_value_vec_initc(&args, fn->params.size);

        if (!_fl_cmp_const_reduce_fn_args(node->child->next, cnt, scope, &args))
        {
            fl_value_vec_free(&args);
            return FL_FALSE;
        }

        *reduced_value = fl_fn_variant_call(fn, args);
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
        id = fl_scope_resolve_id(scope, node->data);
        *reduced_value = id->value;
        break;
    case FL_AST_NODE_TYPE_LITERAL:
    {
        fl_value lit_value = *(fl_value *)node->data;
        *reduced_value = lit_value;
        break;
    }
    default:
        assert(FL_FALSE);
        break;
    }

    return FL_TRUE;
}

static inline fl_bool _fl_cmp_const_reduce_fn_args(
    const fl_ast_node *arg_node,
    fl_context *cnt,
    const fl_scope *scope,
    fl_value_vec *reduced_args)
{
    while (arg_node)
    {
        fl_value arg_value;
        if (!_fl_cmp_const_reduce(arg_node, cnt, scope, &arg_value))
            return FL_FALSE;

        fl_value_vec_push(reduced_args, arg_value);
        arg_node = arg_node->next;
    }

    return FL_TRUE;
}

static inline fl_fn_variant *_fl_cmp_infer_fn_overload(
    const fl_ast_node *arg_node,
    const fl_scope *scope,
    const fl_fn_variant_vec *variants)
{
    fl_value_desc_vec v_descs;
    fl_value_desc_vec_init(&v_descs);

    while (arg_node)
    {
        fl_value_desc vd = _fl_cmp_deduce_value_desc(arg_node, scope);
        fl_value_desc_vec_push(&v_descs, vd);
        arg_node = arg_node->next;
    }

    fl_fn_variant *match = fl_fn_variant_vec_match(variants, v_descs);
    fl_value_desc_vec_free(&v_descs);
    return match;
}