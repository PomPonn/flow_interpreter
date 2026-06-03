/**
 * ---------- compiler.c --------------
 *
 * Implements compiler logic.
 *
 * ------------------------------------
 */

#include "compiler.h"

// ---------- DEPENDENCIES ----------

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
#include "flow/section_mark.h"
#include "flow/operation.h"
#include "vecs/instr_alias_vec.h"
#include "flow/instr/g_instrs.h"
#include "interpreter/interpreter.h"
#include "context/cmp_context.h"
#include "tools/time.h"

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

fl_void _fl_cmp_compile_node(const fl_ast_node *node, fl_cmp_context *cnt);

inline fl_void _fl_cmp_compile_block(const fl_ast_node *node, fl_cmp_context *cnt);

fl_void _fl_cmp_compile_op_call(const fl_ast_node *op_node, fl_cmp_context *cnt);

fl_void _fl_cmp_compile_plain_id_chain(const fl_ast_node *node, fl_cmp_context *cnt);

inline fl_void _fl_cmp_compile_literal(const fl_ast_node *node, fl_cmp_context *cnt);

inline fl_param _fl_cmp_parse_node_to_param(const fl_ast_node *node, const fl_cmp_context *cnt);

fl_param_vec _fl_cmp_parse_nodes_to_params(const fl_ast_node *arg_node, const fl_cmp_context *cnt);

inline fl_void _fl_cmp_byte_code_push_id_access(fl_byte_vec *code, fl_uint offset, fl_int_vec inds);

inline fl_void _fl_cmp_compile_args(const fl_ast_node *arg_node, fl_cmp_context *cnt);

inline fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt);

fl_operation _fl_cmp_id_node_to_operation(const fl_ast_node *node, const fl_scope *scope);

inline fl_void _fl_cmp_paste_bytecode_header(fl_byte_vec *bytes);

inline fl_size _fl_cmp_make_literal(fl_value lit_value, fl_cmp_context *cnt);

fl_value_vec _fl_cmp_parse_nodes_to_values(fl_ast_node *arg_node, fl_cmp_context *cnt);

// ---------- DEFINITIONS ----------

fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *tree, fl_base_context *base)
{
    // init blank context
    fl_cmp_context cnt;
    fl_cmp_context_init(&cnt, base);

    // compile child nodes
    _fl_cmp_compile_node(tree, &cnt);

    fl_byte_vec final;
    fl_byte_vec_init(&final);

    // add byte code version info
    _fl_cmp_paste_bytecode_header(&final);

    if (!fl_value_vec_is_empty(&cnt.base->literals))
    {
        // add literals table
        fl_byte_vec_push(&final, FL_SECTION_MARK_LITERALS);
        fl_byte_vec_push_t(&final, cnt.base->literals.size, fl_size);
        fl_value_vec_to_bytes(&cnt.base->literals, &final);
    }

    // merge both byte vecs
    fl_byte_vec_push(&final, FL_SECTION_MARK_CODE);
    fl_byte_vec_merge(&final, &cnt.byte_code);

    // free remains
    // cnt.byte_code is freed
    fl_cmp_context_free(&cnt);
    return final;
}

fl_void _fl_cmp_compile_node(const fl_ast_node *node, fl_cmp_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_START_BLOCK);
        _fl_cmp_compile_block(node, cnt);
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_END_BLOCK);
        break;
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
        // operation
        _fl_cmp_compile_op_call(node, cnt);
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
        _fl_cmp_compile_plain_id_chain(node, cnt);
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        _fl_cmp_compile_literal(node, cnt);
        break;
    default:
        assert(FL_FALSE);
        break;
    }
}

inline fl_void _fl_cmp_compile_block(const fl_ast_node *node, fl_cmp_context *cnt)
{
    fl_scope *this_scope = fl_mem_alloc_t(fl_scope);
    fl_scope_init(this_scope, cnt->base->scope);

    node = node->child;
    while (node)
    {
        _fl_cmp_compile_node(node, cnt);
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

    if (!id)
        // throw an error if id does not exist
        throw_compilation_error(id_node, "Use of unknown operation id");

    fl_value id_val = id->value;

    if (id_val.data_type != FL_DATA_TYPE_FN)
        // throw an error if id is not fn
        throw_compilation_error(id_node, "id is not a function");

    return fl_operation_from_fn_vars(id_val.data);
}

fl_void _fl_cmp_compile_op_call(const fl_ast_node *op_node, fl_cmp_context *cnt)
{
    fl_ast_node *id_node = op_node->child;
    fl_ast_node *arg_node = id_node->next;
    // id_chain declaration for error throwing
    fl_id_chain *id_chain = id_node->data;
    fl_operation op = _fl_cmp_id_node_to_operation(id_node, cnt->base->scope);

    fl_param_vec params = _fl_cmp_parse_nodes_to_params(arg_node, cnt);
    const fl_fn_interface *fn_i = fl_operation_get_interface(op, params);

    if (!fn_i)
        throw_compilation_error(id_node, "couldn't find function overload for given arguments");

    fl_bool const_pure = fl_bitfield_is_set(fn_i->traits, FL_FN_TRAITS_PURE) && fl_param_vec_is_every_const(&params);
    fl_bool simulate = fl_bitfield_is_set(fn_i->traits, FL_FN_TRAITS_SIMULATE);

    if (!simulate)
        simulate = fl_bitfield_is_set(fn_i->traits, FL_FN_TRAITS_SIMULATE_CASCADE) &&
                   fl_param_vec_cascade_simulate(&params);

    fl_param_vec_free(&params);

    if (simulate || const_pure)
    {
        // call this operation
        fl_value_vec args = _fl_cmp_parse_nodes_to_values(arg_node, cnt);
        fl_value res = fl_operation_run(op, args, cnt->base);

        // does res needs to be freed?

        if (const_pure)
        {
            // inline result

            if (fl_value_equal(res, FL_VALUE_EMPTY))
                assert(FL_FALSE);

            // push immediate value
            fl_byte_vec_push(&cnt->byte_code, FL_INSTR_IMMED_VALUE);
            fl_value_to_bytes(res, &cnt->byte_code);
            // not sure if its always good to free
            fl_value_free(&res);
            return;
        }
    }

    // compile this operator call to byte code
    if (op.type == FL_OPERATION_FN)
    {
        fl_uint offset;
        fl_int_vec inds = fl_scope_find_id_path(cnt->base->scope, id_chain, &offset);
        fl_id_entry *id = fl_scope_traverse(cnt->base->scope, inds, offset);

        // start CALL instruction
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_CALL);
        // push information how to access id
        _fl_cmp_byte_code_push_id_access(&cnt->byte_code, offset, inds);
    }
    else if (op.type == FL_OPERATION_INSTR)
        fl_byte_vec_push(&cnt->byte_code, op.instr);
    else
        assert(FL_FALSE);

    _fl_cmp_compile_args(arg_node, cnt);
}

inline fl_size _fl_cmp_make_literal(fl_value lit_value, fl_cmp_context *cnt)
{
    fl_value_vec *lits = &cnt->base->literals;
    fl_int ind = fl_value_vec_find_identical(lits, lit_value);

    if (ind == FL_FIND_FAIL)
    {
        fl_value_vec_push(lits, lit_value);
        return lits->size - 1;
    }

    return (fl_size)ind;
}

inline fl_void _fl_cmp_compile_literal(const fl_ast_node *node, fl_cmp_context *cnt)
{
    fl_value *value = node->data;
    fl_size index = _fl_cmp_make_literal(*value, cnt);

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_GET_LITERAL);
    fl_byte_vec_push_mem(&cnt->byte_code, &index, sizeof(index));
}

fl_void _fl_cmp_compile_plain_id_chain(const fl_ast_node *node, fl_cmp_context *cnt)
{
    fl_id_chain *id_chain = (fl_id_chain *)node->data;

    fl_uint offset;
    fl_int_vec inds = fl_scope_find_id_path(cnt->base->scope, id_chain, &offset);
    fl_id_entry *id = fl_scope_traverse(cnt->base->scope, inds, offset);

    if (id)
    {
        // push how to information to access id
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_GET_ID);
        _fl_cmp_byte_code_push_id_access(&cnt->byte_code, offset, inds);
    }
    else if (id_chain->size == 1)
    {
        const fl_char *name = id_chain->data[0];
        // handle new id creation
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_NEW_ID);
        fl_byte_vec_push_mem(&cnt->byte_code, name, fl_str_len(name) + 1);
        _fl_cmp_handle_new_id(name, cnt);
    }

    else
        // error: undefined id
        assert(FL_FALSE);

    fl_int_vec_free(&inds);
}

inline fl_param _fl_cmp_parse_node_to_param(const fl_ast_node *node, const fl_cmp_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        return fl_param_create_traited(FL_DATA_TYPE_SCOPE, FL_PARAM_TRAITS_CONST);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_ast_node *id_node = node->child;
        fl_operation op = _fl_cmp_id_node_to_operation(id_node, cnt->base->scope);
        fl_param_vec params = _fl_cmp_parse_nodes_to_params(id_node->next, cnt);
        const fl_fn_interface *fn_i = fl_operation_get_interface(op, params);

        fl_param_traits traits = FL_PARAM_TRAITS_NONE;

        if (fl_param_vec_is_every_const(&params))
            fl_bitfield_set(traits, FL_PARAM_TRAITS_CONST);

        if (fl_bitfield_is_set(fn_i->traits, FL_FN_TRAITS_SIMULATE) ||
            (fl_bitfield_is_set(fn_i->traits, FL_FN_TRAITS_SIMULATE_CASCADE) &&
             fl_param_vec_cascade_simulate(&params)))
            fl_bitfield_set(traits, FL_PARAM_TRAITS_SIMULATE);

        fl_param_vec_free(&params);
        return fl_param_create_traited(fn_i->return_type, traits);
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_id_entry *id = fl_scope_resolve_id(cnt->base->scope, node->data);
        // is const trait valid here?
        if (id)
            return fl_param_create_traited(id->value.data_type, FL_PARAM_TRAITS_REF | FL_PARAM_TRAITS_CONST);
        else
            // do we want to allow use of undefined id chains?
            // (shoud an error be thrown?)
            return fl_param_create_traited(FL_DATA_TYPE_NULL, FL_PARAM_TRAITS_REF);
    }
    case FL_AST_NODE_TYPE_LITERAL:
    {
        fl_value *lit_val = node->data;
        return fl_param_create_traited(lit_val->data_type, FL_PARAM_TRAITS_CONST);
    }
    default:
        assert(FL_FALSE);
        return fl_param_create(FL_DATA_TYPE_NULL);
    }
}

fl_param_vec _fl_cmp_parse_nodes_to_params(const fl_ast_node *arg_node, const fl_cmp_context *cnt)
{
    fl_param_vec params;
    fl_param_vec_init(&params);

    while (arg_node)
    {
        fl_param par = _fl_cmp_parse_node_to_param(arg_node, cnt);
        fl_param_vec_push(&params, par);
        arg_node = arg_node->next;
    }

    return params;
}

inline fl_void _fl_cmp_byte_code_push_id_access(fl_byte_vec *code, fl_uint offset, fl_int_vec inds)
{
    // push scope back offset
    fl_byte_vec_push_mem(code, &offset, sizeof(offset));
    // push size of indexes
    fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
    // push actual indexes
    fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);
}

inline fl_void _fl_cmp_compile_args(const fl_ast_node *arg_node, fl_cmp_context *cnt)
{
    while (arg_node)
    {
        _fl_cmp_compile_node(arg_node, cnt);
        arg_node = arg_node->next;
    }

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_END_ARGS);
}

inline fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt)
{
    // create dummy id in current scope
    fl_id_entry *id;
    fl_id_entry_init(&id, fl_str_dup(name), FL_DATA_TYPE_ANY, FL_VALUE_EMPTY, FL_FALSE);
    fl_id_entry_vec_push(&cnt->base->scope->ids, id);

    return id;
}

inline fl_void _fl_cmp_paste_bytecode_header(fl_byte_vec *bytes)
{
    fl_byte_vec_push(bytes, FL_SECTION_MARK_HEADER);

    // push byte code version
    // size + 1 for '\0' (end of string)
    fl_byte_vec_push_mem(bytes, _BYTECODE_VERSION, fl_str_len(_BYTECODE_VERSION) + 1);

    // add compilation timestamp
    fl_long timestamp = fl_time_get_unix();
    fl_byte_vec_push_t(bytes, timestamp, fl_long);
}

fl_value_vec _fl_cmp_parse_nodes_to_values(fl_ast_node *arg_node, fl_cmp_context *cnt)
{
    fl_value_vec main_args;
    fl_value_vec_init(&main_args);

    while (arg_node)
    {
        switch (arg_node->type)
        {
        case FL_AST_NODE_TYPE_BLOCK:
        {
            // TODO ...
            assert(FL_FALSE);

            fl_scope *inner = fl_mem_alloc_t(fl_scope);
            fl_scope_init(inner, cnt->base->scope);
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
            fl_ast_node *id_node = arg_node->child;
            fl_ast_node *inner_arg_node = id_node->data;
            fl_operation op = _fl_cmp_id_node_to_operation(id_node, cnt->base->scope);
            fl_value_vec args = _fl_cmp_parse_nodes_to_values(id_node->next, cnt);

            fl_value res = fl_operation_run(op, args, cnt->base);
            fl_value_vec_push(&main_args, res);
            break;
        }
        case FL_AST_NODE_TYPE_ID_CHAIN:
        {
            fl_id_chain *id_chain = arg_node->data;
            fl_id_entry *id = fl_scope_resolve_id(cnt->base->scope, id_chain);

            if (!id && id_chain->size == 1)
            {
                fl_value ref = fl_value_create(FL_DATA_TYPE_REF, _fl_cmp_handle_new_id(id_chain->data[0], cnt));
                fl_value_vec_push(&main_args, ref);
            }
            else
                fl_value_vec_push(&main_args, id ? id->value : FL_VALUE_EMPTY);

            break;
        }
        case FL_AST_NODE_TYPE_LITERAL:
        {
            fl_value *lit_val = (fl_value *)arg_node->data;
            fl_value_vec_push(&main_args, fl_value_copy(*lit_val));
            break;
        }
        default:
            assert(FL_FALSE);
            break;
        }

        arg_node = arg_node->next;
    }

    return main_args;
}
