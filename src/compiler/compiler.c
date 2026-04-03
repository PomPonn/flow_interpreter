/**
 * ---------- compiler.c --------------
 *
 * Implements compiler logic.
 *
 * ------------------------------------
 */

#include "compiler.h"

// ---------- DEPENDENCIES ----------

#include "shared/error.h"
#include "shared/str.h"
#include "tokenizer/tokenizer.h"
#include "optimalizer/optimalizer.h"
#include "ast/ast_tree.h"
#include "revise_me/id/id_node.h"
#include "vecs/fn_variant_vec.h"
#include "vecs/const_cstr_vec.h"
#include "lang/scope.h"
#include "lang/value_desc.h"
#include "lang/value.h"
#include "lang/instructions.h"
#include "tools/file.h"
#ifndef NDEBUG
#include "tools/printer.h"
#include <stdio.h>
#endif

// ---------- MACROS ----------------

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

// ---------- DECLARATIONS ----------

// TODO: comment these declarations when finished

static fl_void _fl_cmp_compile_node(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static inline fl_void _fl_cmp_compile_block(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static fl_void _fl_cmp_compile_fn_call(
    const fl_ast_node *cll_node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *codes);

static fl_void _fl_cmp_compile_plain_id_chain(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static inline fl_void _fl_cmp_compile_literal(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static inline fl_value _fl_cmp_try_resolve_predef_const(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope);

static fl_bool _fl_cmp_try_compile_predef(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

static inline fl_bool _fl_cmp_verify_predef(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope);

static inline fl_predef *_fl_cmp_get_predef(
    const fl_id_chain *id_chain,
    fl_context *cnt);

static inline fl_value_desc
_fl_cmp_deduce_node_value_desc(
    const fl_ast_node *node,
    const fl_context *cnt,
    const fl_scope *scope);

// try to reduce given ast node to some constant value
static inline fl_bool _fl_cmp_resolve_node_const(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_value *reduced_value,
    fl_id_entry **assoc_id);

static fl_value_vec _fl_cmp_resolve_args_const(
    const fl_ast_node *arg_node,
    fl_context *cnt,
    fl_scope *scope,
    const fl_value_desc_vec expected);

static inline fl_fn_variant *_fl_cmp_infer_fn_overload(
    const fl_ast_node *arg_node,
    const fl_context *cnt,
    const fl_scope *scope,
    const fl_fn_variant_vec *variants);

static fl_value_desc_vec _fl_cmp_get_args_preview(
    const fl_ast_node *arg_node,
    const fl_context *cnt,
    const fl_scope *scope);

// ---------- DEFINITIONS ----------

fl_void fl_cmp_compile(fl_char *source_path, fl_context *cnt)
{
    fl_char *source_code = fl_file_read(source_path);

    if (!source_code)
        throw_file_error("Couldn't read the file (does it exist?)");

    printf("Kod zrodlowy:\n%s\n\n", source_code);

    fl_token_arr token_arr;
    fl_const_cstr_vec predef_escapes = fl_predef_vec_escapes_view(&cnt->predefines);
    // fl_const_cstr_vec predef_escapes = fl_predef_vec_escapes_view(&cnt->predefines);
    fl_bool any_token = fl_tok_tokenize(source_code, &token_arr, &predef_escapes, source_path);
    fl_const_cstr_vec_free(&predef_escapes);

#ifndef NDEBUG
    printf("Tokeny:\n");
    fl_prt_print_token_entries(&token_arr);
#endif

    fl_opt_remove_comments(&token_arr);

    fl_file_free(&source_code);

    if (!any_token)
        // there are no meaningful tokens in the file
        return;

    fl_ast_node *tree = fl_ast_tree_gen(&token_arr);

#ifndef NDEBUG
    printf("Wezly drzewa skladniowego:\n");
    fl_prt_print_ast_tree(tree);
#endif

    fl_byte_vec code;
    fl_byte_vec_init(&code);

    _fl_cmp_compile_node(tree, cnt, &cnt->main_scope, &code);

    fl_token_arr_free(&token_arr);
    fl_ast_node_free(&tree);
}

static fl_void _fl_cmp_compile_node(
    const fl_ast_node *node,
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
        _fl_cmp_compile_fn_call(node, cnt, scope, code);
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
    const fl_ast_node *node,
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

static fl_void _fl_cmp_compile_fn_call(
    const fl_ast_node *cll_node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_ast_node *id_node = cll_node->child;

    fl_predef *predef = _fl_cmp_get_predef(id_node->data, cnt);
    if (predef)
    {
        if (!_fl_cmp_try_compile_predef(id_node, predef, cnt, scope, code))
            assert(FL_FALSE);

        return;
    }

    fl_id_chain *id_chain = (fl_id_chain *)id_node->data;
    fl_uint offset;
    fl_int_vec inds = fl_scope_find_id_path(scope, id_chain, &offset);
    fl_id_entry *id = fl_scope_traverse(scope, inds, offset);
    fl_ast_node *arg_node = id_node->next;

    if (!id)
        // throw an error if id does not exist
        throw_compilation_error(id_node, "Use of unknown function id");

    if (id->value.data_type != FL_DATA_TYPE_FN)
        // throw an error if id is not fn
        throw_compilation_error(id_node, "id is not a function");

    // deduce function argument types
    fl_fn_variant *fn_var = _fl_cmp_infer_fn_overload(arg_node, cnt, scope, id->value.data);

    // TODO: better error message needed
    if (!fn_var)
        throw_compilation_error(id_node, "couldn't find function overload for given arguments");

    // try to get const args to expand them at compile time
    fl_value_vec const_args = _fl_cmp_resolve_args_const(arg_node, cnt, scope, fn_var->params);

    if (fl_value_vec_is_empty(&const_args))
    {
        fl_value res = fl_fn_variant_call(fn_var, const_args);

        // insert resulting value to the code

        fl_value_vec_free(&const_args);
        return;
    }

    // non-constant arguments in constant function call
    if (id->constant)
        assert(FL_FALSE);

    // compile this function call to byte code

    // start CALL instruction
    fl_byte_vec_push(code, FL_INSTR_CALL);
    // push information how to access id
    // push scope back offset
    fl_byte_vec_push_mem(code, &offset, sizeof(offset));
    // push size of indexes
    fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
    // push actual indexes
    fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);

    // compile args
    while (arg_node)
    {
        _fl_cmp_compile_node(arg_node, cnt, scope, code);
        arg_node = arg_node->next;
    }
}

static fl_bool _fl_cmp_try_compile_predef(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    // try to resolve this predef in compile time
    fl_value res_value = _fl_cmp_try_resolve_predef_const(id_node, predef, cnt, scope);
    if (!fl_value_is_empty(res_value))
    {
        // append resolved value to the code
        fl_value_to_bytes(res_value, code);
        return FL_TRUE;
    }

    // else compile it normally

    fl_ast_node *arg_node = id_node->next;

    // if it's constant, args were wrong, throw an error
    if (fl_predef_is_constant(predef))
    {
        assert(FL_FALSE);
    }

    // compile this predefine call to byte code

    // push instruction
    fl_byte_vec_push(code, predef->type);

    // compile args
    while (arg_node)
    {
        _fl_cmp_compile_node(arg_node, cnt, scope, code);
        arg_node = arg_node->next;
    }

    return FL_TRUE;
}

static fl_value _fl_cmp_try_resolve_predef_const(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope)
{
    if (!_fl_cmp_verify_predef(id_node->next, predef, cnt, scope))
        assert(FL_FALSE);

    const fl_value_desc_vec expected_params = fl_predef_get_client_params(predef);

    fl_ast_node *arg_node = id_node->next;
    // try to get const args to expand them at compile time
    fl_value_vec const_args = _fl_cmp_resolve_args_const(arg_node, cnt, scope, expected_params);

    if (!fl_value_vec_is_empty(&const_args))
    {
        fl_value res = fl_predef_call(predef, cnt, const_args);

        // what to do with this freeing? Is it alwas safe not to free?
        // fl_value_vec_free(&const_args);

        return res;
    }

    return FL_VALUE_EMPTY;
}

static inline fl_bool _fl_cmp_verify_predef(
    const fl_ast_node *arg_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope)
{
    const fl_value_desc_vec expected_params = fl_predef_get_client_params(predef);
    fl_value_desc_vec args_view = _fl_cmp_get_args_preview(arg_node, cnt, scope);

    // parameters are not matching
    if (!fl_value_desc_vec_equal(&expected_params, &args_view))
        return FL_FALSE;

    return FL_TRUE;
}

static inline fl_predef *_fl_cmp_get_predef(
    const fl_id_chain *id_chain,
    fl_context *cnt)
{
    if (id_chain->size != 1)
        return FL_NULL;

    fl_uint predef_ind = fl_predef_vec_find(&cnt->predefines, id_chain->data[0]);
    if (predef_ind == FL_FIND_FAIL)
        return FL_NULL;

    return &cnt->predefines.data[predef_ind];
}

static inline fl_void _fl_cmp_compile_literal(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_value *value = node->data;
    fl_uint index = fl_context_make_literal(cnt, *value);

    fl_byte_vec_push(code, FL_INSTR_GET_LITERAL);
    fl_byte_vec_push_mem(code, &index, sizeof(index));
}

static fl_void _fl_cmp_compile_plain_id_chain(
    const fl_ast_node *node,
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
        fl_byte_vec_push(code, FL_INSTR_NEW_ID);

        // create dummy id in this scope
        fl_id_entry *entry;
        fl_id_entry_init(&entry, fl_str_dup(id_chain->data[0]), FL_DATA_TYPE_ANY, FL_VALUE_EMPTY, FL_FALSE);
        fl_id_entry_vec_push(&scope->ids, entry);
    }
    else
    {
        // error: undefined id
        assert(FL_FALSE);
    }

    fl_int_vec_free(&inds);
}

static inline fl_value_desc _fl_cmp_deduce_node_value_desc(
    const fl_ast_node *node,
    const fl_context *cnt,
    const fl_scope *scope)
{
    fl_id_entry *id = FL_NULL;

    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        return fl_value_desc_create(FL_DATA_TYPE_SCOPE, FL_FALSE);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_id_chain *id_chain = node->child->data;

        // check if it's predef
        if (id_chain->size == 1)
        {
            fl_uint predef_ind = fl_predef_vec_find(&cnt->predefines, id_chain->data[0]);
            if (predef_ind != FL_FIND_FAIL)
            {
                fl_data_type type = fl_predef_get_return_type(&cnt->predefines.data[predef_ind]);
                return fl_value_desc_create(type, FL_FALSE);
            }
        }

        // it's a function otherwise
        id = fl_scope_resolve_id(scope, id_chain);
        assert(id->value.data_type == FL_DATA_TYPE_FN);

        fl_ast_node *arg_node = node->child->next;
        fl_fn_variant *var = _fl_cmp_infer_fn_overload(arg_node, cnt, scope, id->value.data);
        return fl_value_desc_create(var->return_type, FL_FALSE);
    }
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

static inline fl_bool _fl_cmp_resolve_node_const(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_value *reduced_value,
    fl_id_entry **assoc_id)
{
    fl_id_entry *id = FL_NULL;

    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
    {
        fl_scope *inner = fl_mem_alloc(sizeof(fl_scope));
        fl_scope_init(inner, scope);

        assert(FL_FALSE);
        // ???

        // fl_ast_node *scope_node = node->child;
        // while (scope_node)
        // {
        //     fl_value reduced;
        //     if (!_fl_cmp_resolve_node_const(scope_node, cnt, &inner, &reduced))
        //     {
        //         fl_scope_free(scope_node);
        //         return FL_FALSE;
        //     }
        // }

        if (reduced_value)
            *reduced_value = fl_value_create(FL_DATA_TYPE_SCOPE, inner);
        break;
    }
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_ast_node *id_node = node->child;
        fl_ast_node *arg_node = id_node->data;
        fl_id_chain *id_chain = id_node->data;

        // check try predef
        fl_predef *predef = _fl_cmp_get_predef(id_chain, cnt);
        if (predef)
        {
            fl_value res = _fl_cmp_try_resolve_predef_const(id_node, predef, cnt, scope);
            if (fl_value_is_empty(res))
                return FL_FALSE;
            else
                *reduced_value = res;

            return FL_TRUE;
        }

        id = fl_scope_resolve_id(scope, id_chain);

        // error?
        if (id->value.data_type != FL_DATA_TYPE_FN || !id->constant)
            return FL_FALSE;

        fl_fn_variant *fn = _fl_cmp_infer_fn_overload(arg_node, cnt, scope, id->value.data);

        // error?
        if (!fn)
            return FL_FALSE;

        fl_value_vec args = _fl_cmp_resolve_args_const(arg_node, cnt, scope, fn->params);

        if (fl_value_vec_is_empty(&args))
            return FL_FALSE;

        if (reduced_value)
            *reduced_value = fl_fn_variant_call(fn, args);
        break;
    }

    case FL_AST_NODE_TYPE_ID_CHAIN:
        id = fl_scope_resolve_id(scope, node->data);

        if (!id)
        {
            // if value is unimportant, it's ok that id does not exist
            if (reduced_value)
                return FL_FALSE;

            // create an id to fulfill the requirement
            fl_id_chain *id_chain = node->data;
            // new id must consist of only one-part name
            if (id_chain->size != 1)
                return FL_FALSE;

            // create new empty id
            fl_id_entry_init(&id, fl_str_dup(id_chain->data[0]), FL_DATA_TYPE_ANY, FL_VALUE_EMPTY, FL_FALSE);
            fl_id_entry_vec_push(&scope->ids, id);
        }
        else
        {
            // error?
            if (!id->constant)
                return FL_FALSE;

            if (reduced_value)
                *reduced_value = id->value;
        }
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        if (reduced_value)
            *reduced_value = *(fl_value *)node->data;
        break;
    default:
        assert(FL_FALSE);
        break;
    }

    *assoc_id = id;
    return FL_TRUE;
}

static inline fl_fn_variant *_fl_cmp_infer_fn_overload(
    const fl_ast_node *arg_node,
    const fl_context *cnt,
    const fl_scope *scope,
    const fl_fn_variant_vec *variants)
{
    fl_value_desc_vec args_preview = _fl_cmp_get_args_preview(arg_node, cnt, scope);
    fl_fn_variant *match = fl_fn_variant_vec_match(variants, args_preview);
    fl_value_desc_vec_free(&args_preview);
    return match;
}

static fl_value_vec _fl_cmp_resolve_args_const(
    const fl_ast_node *arg_node,
    fl_context *cnt,
    fl_scope *scope,
    const fl_value_desc_vec expected)
{
    fl_value_vec args;
    fl_value_vec_init(&args);

    for (fl_size i = 0; i < expected.size; i++)
    {
        fl_value_desc curr_exp = expected.data[i];
        fl_value arg_value;
        fl_id_entry *assoc_id;
        if (!_fl_cmp_resolve_node_const(
                arg_node, cnt, scope,
                curr_exp.refable ? FL_NULL : &arg_value, &assoc_id))
        {
            fl_value_vec_free(&args);
            return args;
        }

        if (expected.data[i].refable)
        {
            if (!assoc_id)
                assert(FL_FALSE);

            arg_value = fl_value_create(FL_DATA_TYPE_REF, assoc_id);
        }

        if (!fl_data_type_has(expected.data[i].type, arg_value.data_type))
            assert(FL_FALSE);

        fl_value_vec_push(&args, arg_value);
        arg_node = arg_node->next;
    }

    // excess args
    if (arg_node)
    {
        assert(FL_FALSE);
    }

    return args;
}

static fl_value_desc_vec _fl_cmp_get_args_preview(
    const fl_ast_node *arg_node,
    const fl_context *cnt,
    const fl_scope *scope)
{
    fl_value_desc_vec v_descs;
    fl_value_desc_vec_init(&v_descs);

    while (arg_node)
    {
        fl_value_desc vd = _fl_cmp_deduce_node_value_desc(arg_node, cnt, scope);
        fl_value_desc_vec_push(&v_descs, vd);
        arg_node = arg_node->next;
    }

    return v_descs;
}