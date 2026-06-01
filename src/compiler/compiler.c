/**
 * ---------- compiler.c --------------
 *
 * Implements compiler logic.
 *
 * ------------------------------------
 */

#include "compiler.h"

// ---------- DEPENDENCIES ----------

#include "flow/context.h"
#include "ast/ast_node.h"
#include "shared/error.h"
#include "shared/str.h"
#include "vecs/fn_variant_vec.h"
#include "ast/ast_node.h"
#include "vecs/const_cstr_vec.h"
#include "flow/scope.h"
#include "flow/fn/param.h"
#include "flow/value.h"
#include "flow/instr/instruction.h"
#include "flow/section.h"
#include "flow/operation.h"
#include "vecs/instr_alias_vec.h"
#include "flow/instr/g_instrs.h"

// ---------- MACROS ----------------

/**
 * @brief Size of id chain string buffer.
 */
#define _ID_STRING_BUFFER_SIZE 512

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

fl_void _fl_cmp_compile_node(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

inline fl_void _fl_cmp_compile_block(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

fl_void _fl_cmp_compile_op_call(
    const fl_ast_node *op_node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *codes);

fl_void _fl_cmp_compile_plain_id_chain(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

inline fl_void _fl_cmp_compile_literal(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_byte_vec *code);

inline fl_value _fl_cmp_try_resolve_predef_const(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope);

fl_bool _fl_cmp_try_compile_predef(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

inline fl_bool _fl_cmp_verify_predef(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope);

inline fl_predef *_fl_cmp_get_predef(
    const fl_id_chain *id_chain,
    fl_context *cnt);

// try to reduce given ast node to some constant value
inline fl_bool _fl_cmp_resolve_node_const(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_value *reduced_value,
    fl_id_entry **assoc_id);

fl_value_vec _fl_cmp_resolve_args_const(
    const fl_ast_node *arg_node,
    fl_context *cnt,
    fl_scope *scope,
    const fl_param_vec expected);

inline fl_fn_variant *_fl_cmp_infer_fn_overload(
    const fl_ast_node *arg_node,
    const fl_context *cnt,
    const fl_scope *scope,
    const fl_fn_variant_vec *variants);

inline fl_param _fl_cmp_node_infer_to_param(
    const fl_ast_node *node,
    const fl_context *cnt,
    const fl_scope *scope);

fl_param_vec _fl_cmp_nodes_infer_to_params(
    const fl_ast_node *arg_node,
    const fl_context *cnt,
    const fl_scope *scope);

inline fl_void _fl_cmp_push_id_access(
    fl_byte_vec *code,
    fl_uint offset,
    fl_int_vec inds);

inline fl_void _fl_cmp_compile_args(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

inline fl_void _fl_cmp_handle_new_id(
    const fl_char *name,
    fl_scope *scope,
    fl_byte_vec *code);

inline fl_void _fl_cmp_compile_args_with_new_id_as_first_arg(
    const fl_ast_node *node,
    const fl_char *new_id_name,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code);

fl_operation _fl_cmp_id_node_to_operation(
    const fl_ast_node *node,
    const fl_scope *scope);

inline fl_void
_fl_cmp_paste_bytecode_version(fl_byte_vec *bytes);

// ---------- DEFINITIONS ----------

fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *tree)
{
    // init blank context
    fl_context cnt;
    fl_context_init(&cnt);
    fl_global_put__predefs(&cnt);

    fl_byte_vec code;
    fl_byte_vec_init(&code);

    _fl_cmp_compile_node(tree, &cnt, &cnt.main_scope, &code);

    fl_byte_vec final;
    fl_byte_vec_init(&final);

    // add byte code version info
    fl_byte_vec_push(&final, FL_SECTION_VERSION);
    _fl_cmp_paste_bytecode_version(&final);

    // add literals table to the start of the byte code
    if (!fl_value_vec_is_empty(&cnt.literals))
    {
        fl_byte_vec_push(&final, FL_SECTION_LITERALS);
        fl_byte_vec_push_t(&final, cnt.literals.size, fl_size);
        fl_value_vec_to_bytes(&cnt.literals, &final);
    }

    // merge both bytes vecs
    fl_byte_vec_push(&final, FL_SECTION_CODE);
    fl_byte_vec_merge(&final, &code);

    // free remains
    fl_byte_vec_free(&code);
    fl_context_free(&cnt);

    return final;
}

fl_void _fl_cmp_compile_node(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        fl_byte_vec_push(code, FL_INSTR_START_BLOCK);
        _fl_cmp_compile_block(node, cnt, scope, code);
        fl_byte_vec_push(code, FL_INSTR_END_BLOCK);
        break;
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
        // operation
        _fl_cmp_compile_op_call(node, cnt, scope, code);
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
        _fl_cmp_compile_plain_id_chain(node, cnt, scope, code);
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        _fl_cmp_compile_literal(node, cnt, code);
        break;
    default:
        assert(FL_FALSE);
        break;
    }
}

inline fl_void _fl_cmp_compile_block(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_scope *this_scope = fl_mem_alloc_t(fl_scope);
    fl_scope_init(this_scope, scope);

    node = node->child;
    while (node)
    {
        _fl_cmp_compile_node(node, cnt, this_scope, code);
        node = node->next;
    }
}

fl_operation _fl_cmp_id_node_to_operation(
    const fl_ast_node *id_node,
    const fl_scope *scope)
{
    fl_id_chain *id_chain = id_node->data;
    const fl_instr_alias *alias = id_chain->size == 1 ? fl_g_instrs_find_alias(id_chain->data[0]) : FL_NULL;

    if (alias)
        return fl_operation_from_instr(alias->instr);

    fl_id_entry *id = fl_scope_resolve_id(scope, id_chain);
    fl_value id_val = id->value;

    if (!id)
        // throw an error if id does not exist
        throw_compilation_error(id_node, "Use of unknown function id");

    if (id_val.data_type != FL_DATA_TYPE_FN)
        // throw an error if id is not fn
        throw_compilation_error(id_node, "id is not a function");

    return fl_operation_from_fn_vars(id_val.data);
}

fl_void _fl_cmp_compile_op_call(
    const fl_ast_node *op_node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_ast_node *id_node = op_node->child;
    // declaration for error throwing
    fl_ast_node *id_chain = id_node->data;
    fl_operation op = _fl_cmp_id_node_to_operation(id_node, scope);

    // REMOVE BELOW

    // // check if it's predef
    // fl_predef *predef = _fl_cmp_get_predef(id_node->data, cnt);
    // if (predef)
    // {
    //     // compile it
    //     if (!_fl_cmp_try_compile_predef(id_node, predef, cnt, scope, code))
    //         assert(FL_FALSE);

    //     return;
    // }

    // // else it has to be a normal function

    // fl_id_chain *id_chain = (fl_id_chain *)id_node->data;
    // fl_uint offset;
    // fl_int_vec inds = fl_scope_find_id_path(scope, id_chain, &offset);
    // fl_id_entry *id = fl_scope_traverse(scope, inds, offset);
    // fl_ast_node *arg_node = id_node->next;

    // if (!id)
    //     // throw an error if id does not exist
    //     throw_compilation_error(id_node, "Use of unknown function id");

    // if (id->value.data_type != FL_DATA_TYPE_FN)
    //     // throw an error if id is not fn
    //     throw_compilation_error(id_node, "id is not a function");

    fl_param_vec params = _fl_cmp_nodes_infer_to_params(id_node->next, cnt, scope);
    const fl_fn_interface *fn_i = fl_operation_get_interface(op, params);

    if (!fn_i)
        throw_compilation_error(id_node, "couldn't find function overload for given arguments");

    if (fn_i->traits.is_pure)
    {
        // Try to inline this calls result
    }

    // // deduce function argument types
    // fl_fn_variant *fn_var = _fl_cmp_infer_fn_overload(arg_node, cnt, scope, id->value.data);

    // // TODO: better error message needed
    // if (!fn_var)
    //     throw_compilation_error(id_node, "couldn't find function overload for given arguments");

    if (id->constant)
    {
        // try to get const args to expand them at compile time
        fl_value_vec const_args = _fl_cmp_resolve_args_const(arg_node, cnt, scope, fn_var->interface.params);

        if (!fl_value_vec_is_empty(&const_args))
        {
            fl_value res = fl_fn_variant_call(fn_var, const_args);

            // insert resulting value to the code
            fl_value_to_bytes(res, code);

            fl_value_vec_free(&const_args);
            return;
        }
    }

    // compile this function call to byte code

    // start CALL instruction
    fl_byte_vec_push(code, FL_INSTR_CALL);
    // push information how to access id
    _fl_cmp_push_id_access(code, offset, inds);

    // compile args
    _fl_cmp_compile_args(arg_node, cnt, scope, code);
}

fl_bool _fl_cmp_try_compile_predef(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    const fl_char *new_id_name = FL_NULL;
    fl_ast_node *arg_node = id_node->next;

    if (predef->instr == FL_INSTR_ASSIGN)
    {
        // create id if it does not exist before further processing
        fl_param param = _fl_cmp_node_infer_to_param(arg_node, cnt, scope);
        if (param.refering == FL_TRUE && param.type == FL_DATA_TYPE_NULL)
        {
            fl_id_chain *id_chain = arg_node->data;

            if (id_chain->size != 1)
                assert(FL_FALSE);

            // create new undefined id
            new_id_name = (id_chain)->data[0];
            fl_id_entry *id;
            fl_id_entry_init(&id, fl_str_dup(new_id_name), FL_DATA_TYPE_ANY, FL_VALUE_EMPTY, FL_FALSE);
            fl_id_entry_vec_push(&scope->ids, id);

            arg_node = arg_node->next;
        }
    }

    // try resolving this predef in compile time
    fl_value res_value = _fl_cmp_try_resolve_predef_const(id_node, predef, cnt, scope);

    // expand resolved value if this predef is inlineable and there is any value
    if (fl_predef_is_inlineable(predef) && !fl_value_is_empty(res_value))
    {
        fl_byte_vec_push(code, FL_INSTR_IMMED_VALUE);
        fl_value_to_bytes(res_value, code);
        return FL_TRUE;
    }

    // push instruction
    fl_byte_vec_push(code, predef->instr);

    // compile args
    if (new_id_name)
        _fl_cmp_compile_args_with_new_id_as_first_arg(arg_node, new_id_name, cnt, scope, code);
    else
        _fl_cmp_compile_args(arg_node, cnt, scope, code);

    return FL_TRUE;
}

fl_value _fl_cmp_try_resolve_predef_const(
    const fl_ast_node *id_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope)
{
    if (!_fl_cmp_verify_predef(id_node->next, predef, cnt, scope))
        assert(FL_FALSE);

    const fl_param_vec expected_params = fl_predef_get_interface(predef)->params;

    fl_ast_node *arg_node = id_node->next;
    // try to get const args to expand them at compile time
    fl_value_vec const_args = _fl_cmp_resolve_args_const(arg_node, cnt, scope, expected_params);

    if (!fl_value_vec_is_empty(&const_args))
    {
        fl_value res = fl_predef_call(predef, cnt, const_args);

        // add this scope to parents inner scopes if it has no parent yet
        if (res.data_type == FL_DATA_TYPE_SCOPE)
        {
            fl_scope *res_scope = (fl_scope *)res.data;
            if (!res_scope->outer)
                fl_scope_add_inner(scope, res_scope);
        }

        // is it safe???
        fl_value_vec_free(&const_args);

        return res;
    }

    return FL_VALUE_EMPTY;
}

inline fl_bool _fl_cmp_verify_predef(
    const fl_ast_node *arg_node,
    const fl_predef *predef,
    fl_context *cnt,
    fl_scope *scope)
{
    const fl_param_vec expected_params = fl_predef_get_interface(predef)->params;
    fl_param_vec args_view = _fl_cmp_nodes_infer_to_params(arg_node, cnt, scope);

    // parameters are not matching
    if (!fl_param_vec_equal(&expected_params, &args_view))
        return FL_FALSE;

    return FL_TRUE;
}

inline fl_predef *_fl_cmp_get_predef(
    const fl_id_chain *id_chain,
    fl_context *cnt)
{
    return id_chain->size != 1 ? FL_NULL : fl_predef_vec_resolve(&cnt->predefines, id_chain->data[0]);
}

inline fl_void _fl_cmp_compile_literal(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_byte_vec *code)
{
    fl_value *value = node->data;
    fl_size index = fl_context_make_literal(cnt, *value);

    fl_byte_vec_push(code, FL_INSTR_GET_LITERAL);
    fl_byte_vec_push_mem(code, &index, sizeof(index));
}

fl_void _fl_cmp_compile_plain_id_chain(
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
        // push how to information to access id
        fl_byte_vec_push(code, FL_INSTR_GET_ID);
        _fl_cmp_push_id_access(code, offset, inds);
    }
    else if (id_chain->size == 1)
        // handle new id
        _fl_cmp_handle_new_id(id_chain->data[0], scope, code);
    else
    {
        // error: undefined id
        assert(FL_FALSE);
    }

    fl_int_vec_free(&inds);
}

inline fl_param _fl_cmp_node_infer_to_param(
    const fl_ast_node *node,
    const fl_context *cnt,
    const fl_scope *scope)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        return fl_param_create(FL_DATA_TYPE_SCOPE);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_ast_node *id_node = node->child;
        fl_operation op = _fl_cmp_id_node_to_operation(id_node, scope);
        fl_param_vec params = _fl_cmp_nodes_infer_to_params(id_node->next, cnt, scope);
        const fl_fn_interface *fn_i = fl_operation_get_interface(op, params);
        return fl_param_create(fn_i->return_type);
        fl_param_vec_free(&params);

        // // check if it's predef
        // if (id_chain->size == 1)
        // {
        //     fl_predef *predef = fl_predef_vec_resolve(&cnt->predefines, id_chain->data[0]);
        //     if (predef)
        //     {
        //         fl_data_type type = fl_predef_get_interface(predef)->return_type;
        //         return fl_param_create(type);
        //     }
        // }

        // // it's a function otherwise
        // fl_id_entry *id = fl_scope_resolve_id(scope, id_chain);

        // assert(id);
        // assert(id->value.data_type == FL_DATA_TYPE_FN);

        // fl_ast_node *arg_node = node->child->next;
        // fl_fn_variant *var = _fl_cmp_infer_fn_overload(arg_node, cnt, scope, id->value.data);
        // return fl_param_create(var->interface.return_type);
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_id_entry *id = fl_scope_resolve_id(scope, node->data);
        if (id)
            return fl_param_create_ref(id->value.data_type);
        else
            // do we want to allow use of undefined id chains?
            // (shoud an error be thrown?)
            return fl_param_create_ref(FL_DATA_TYPE_NULL);
    }
    case FL_AST_NODE_TYPE_LITERAL:
    {
        fl_value *lit_val = node->data;
        return fl_param_create(lit_val->data_type);
    }
    default:
        assert(FL_FALSE);
        return fl_param_create(FL_DATA_TYPE_NULL);
    }
}

inline fl_bool _fl_cmp_resolve_node_const(
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
        // TODO ...
        assert(FL_FALSE);

        fl_scope *inner = fl_mem_alloc_t(fl_scope);
        fl_scope_init(inner, scope);

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

        // if (reduced_value)
        //     *reduced_value = fl_value_create(FL_DATA_TYPE_SCOPE, inner);
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

        fl_value_vec args = _fl_cmp_resolve_args_const(arg_node, cnt, scope, fn->interface.params);

        if (fl_value_vec_is_empty(&args))
            return FL_FALSE;

        if (reduced_value)
            *reduced_value = fl_fn_variant_call(fn, args);
        break;
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
        id = fl_scope_resolve_id(scope, node->data);
        if (!id)
            return FL_FALSE;

        // error?
        // if (!id->constant)
        //     return FL_FALSE;

        if (reduced_value)
            *reduced_value = id->value;
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        if (reduced_value)
            *reduced_value = fl_value_copy(*(fl_value *)node->data);
        break;
    default:
        assert(FL_FALSE);
        break;
    }

    *assoc_id = id;
    return FL_TRUE;
}

inline fl_fn_variant *_fl_cmp_infer_fn_overload(
    const fl_ast_node *arg_node,
    const fl_context *cnt,
    const fl_scope *scope,
    const fl_fn_variant_vec *variants)
{
    fl_param_vec args_preview = _fl_cmp_nodes_infer_to_params(arg_node, cnt, scope);
    fl_fn_variant *match = fl_fn_variant_vec_find_match_p(variants, args_preview);
    fl_param_vec_free(&args_preview);
    return match;
}

fl_value_vec _fl_cmp_resolve_args_const(
    const fl_ast_node *arg_node,
    fl_context *cnt,
    fl_scope *scope,
    const fl_param_vec expected)
{
    fl_value_vec args;
    fl_value_vec_init(&args);

    for (fl_size i = 0; i < expected.size; i++)
    {
        fl_param curr_exp = expected.data[i];
        fl_value arg_value;
        fl_id_entry *assoc_id;
        if (!_fl_cmp_resolve_node_const(
                arg_node, cnt, scope,
                curr_exp.refering ? FL_NULL : &arg_value, &assoc_id))
        {
            fl_value_vec_free(&args);
            return args;
        }

        if (expected.data[i].refering)
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
        assert(FL_FALSE);

    return args;
}

fl_param_vec _fl_cmp_nodes_infer_to_params(
    const fl_ast_node *arg_node,
    const fl_context *cnt,
    const fl_scope *scope)
{
    fl_param_vec params;
    fl_param_vec_init(&params);

    while (arg_node)
    {
        fl_param par = _fl_cmp_node_infer_to_param(arg_node, cnt, scope);
        fl_param_vec_push(&params, par);
        arg_node = arg_node->next;
    }

    return params;
}

inline fl_void _fl_cmp_push_id_access(
    fl_byte_vec *code,
    fl_uint offset,
    fl_int_vec inds)
{
    // push scope back offset
    fl_byte_vec_push_mem(code, &offset, sizeof(offset));
    // push size of indexes
    fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
    // push actual indexes
    fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);
}

inline fl_void _fl_cmp_compile_args(
    const fl_ast_node *node,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    // fl_byte_vec_push(code, FL_INSTR_START_ARGS);
    while (node)
    {
        _fl_cmp_compile_node(node, cnt, scope, code);
        node = node->next;
    }
    fl_byte_vec_push(code, FL_INSTR_END_ARGS);
}

inline fl_void _fl_cmp_handle_new_id(
    const fl_char *name,
    fl_scope *scope,
    fl_byte_vec *code)
{
    fl_byte_vec_push(code, FL_INSTR_NEW_ID);
    fl_byte_vec_push_mem(code, name, fl_str_len(name) + 1);

    // create dummy id in this scope
    fl_id_entry *id;
    fl_id_entry_init(&id, fl_str_dup(name), FL_DATA_TYPE_ANY, FL_VALUE_EMPTY, FL_FALSE);
    fl_id_entry_vec_push(&scope->ids, id);
}

inline fl_void _fl_cmp_compile_args_with_new_id_as_first_arg(
    const fl_ast_node *sec_arg_node,
    const fl_char *new_id_name,
    fl_context *cnt,
    fl_scope *scope,
    fl_byte_vec *code)
{
    assert(new_id_name);

    // fl_byte_vec_push(code, FL_INSTR_START_ARGS);

    fl_byte_vec_push(code, FL_INSTR_NEW_ID);
    fl_byte_vec_push_mem(code, new_id_name, fl_str_len(new_id_name) + 1);

    while (sec_arg_node)
    {
        _fl_cmp_compile_node(sec_arg_node, cnt, scope, code);
        sec_arg_node = sec_arg_node->next;
    }
    fl_byte_vec_push(code, FL_INSTR_END_ARGS);
}

inline fl_void _fl_cmp_paste_bytecode_version(fl_byte_vec *bytes)
{
    // size + 1 for '\0' (end of string)
    fl_byte_vec_push_mem(bytes, _BYTECODE_VERSION, fl_str_len(_BYTECODE_VERSION) + 1);
}
