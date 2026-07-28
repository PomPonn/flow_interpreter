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
#include "vecs/instr_info_vec.h"
#include "flow/instr/g_instrs.h"
#include "interpreter/interpreter.h"
#include "context/cmp_context.h"
#include "flow/id/id_chain.h"
#include "tools/time.h"

// ---------- MACROS ----------------

/**
 * @brief Shorthand for throwing a compilation error.
 */
#define throw_compilation_error(ast_node_ptr, message)                             \
    {                                                                              \
        fl_char buffer[FL_ID_CHAIN_MAX_SIZE];                                      \
        fl_id_chain_to_string(ast_node_ptr->data, buffer, FL_ID_CHAIN_MAX_SIZE);   \
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

inline fl_void _fl_cmp_byte_code_push_id_access_by_inds(fl_byte_vec *code, fl_uint offset, fl_int_vec inds);

inline fl_void _fl_cmp_byte_code_push_id_access_by_names(fl_byte_vec *code, const fl_id_chain *chain);

fl_void _fl_cmp_compile_args(const fl_ast_node *arg_node, fl_cmp_context *cnt);

inline fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt);

fl_operation _fl_cmp_id_node_to_operation(const fl_ast_node *node, const fl_scope *scope);

inline fl_void _fl_cmp_paste_byte_code_header(fl_byte_vec *bytes);

inline fl_size _fl_cmp_make_literal(fl_value lit_value, fl_cmp_context *cnt);

inline fl_value_vec _fl_cmp_parse_nodes_to_values(fl_ast_node *arg_node, fl_cmp_context *cnt);

inline fl_bool _fl_cmp_is_call_const(fl_operation op, const fl_fn_interface *intf, const fl_param_vec actual_params);

fl_value _fl_cmp_parse_node_to_value(fl_ast_node *node, fl_cmp_context *cnt);

fl_value_vec _fl_cmp_eval_args(fl_ast_node *arg_node, fl_operation op, fl_cmp_context *cnt);

fl_bool _fl_cmp_try_op_inline(fl_ast_node *arg_node, fl_operation op, fl_cmp_context *cnt);

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
    _fl_cmp_paste_byte_code_header(&final);

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
    fl_scope *this_scope;
    fl_scope_init(&this_scope, cnt->base->scope);

    node = node->child;
    while (node)
    {
        _fl_cmp_compile_node(node, cnt);
        node = node->next;
    }
}

fl_operation _fl_cmp_id_node_to_operation(const fl_ast_node *id_node, const fl_scope *scope)
{
    fl_id_chain *id_chain = id_node->data;

    fl_instruction instr = id_chain->size == 1 ? fl_instruction_from_alias(id_chain->data[0]) : FL_INSTR_NONE;
    if (instr)
        return fl_operation_from_instr(instr);

    fl_id_entry *id = fl_scope_resolve_id(scope, id_chain);

    if (id)
    {
        fl_value id_val = id->value;
        return fl_operation_from_fn_vars(id_val.data);
    }

    return fl_operation_unknown();
}

fl_void _fl_cmp_compile_op_call(const fl_ast_node *op_node, fl_cmp_context *cnt)
{
    fl_ast_node *id_node = op_node->child;
    fl_ast_node *arg_node = id_node->next;
    fl_id_chain *id_chain = id_node->data;
    fl_operation op = _fl_cmp_id_node_to_operation(id_node, cnt->base->scope);

    if (op.type == FL_OPERATION_UNKNOWN)
    {
        // assume this operation is correct,
        // put responsibility on the interpreter

        // start CALL instruction
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_CALL);
        // push information how to access id
        _fl_cmp_byte_code_push_id_access_by_names(&cnt->byte_code, id_chain);

        _fl_cmp_compile_args(arg_node, cnt);
        return;
    }

    if (!fl_operation_is_callable(op))
    {
        assert(FL_FALSE);
    }

    fl_bool success = _fl_cmp_try_op_inline(id_node, op, cnt);

    if (!success)
    {
        // compile this operator call to byte code
        if (op.type == FL_OPERATION_FN)
        {
            fl_uint offset;
            fl_int_vec inds = fl_scope_find_id_path(cnt->base->scope, id_chain, &offset);
            fl_id_entry *id = fl_scope_traverse(cnt->base->scope, inds, offset);

            // start CALL instruction
            fl_byte_vec_push(&cnt->byte_code, FL_INSTR_CALL);
            // push information how to access id
            _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);
        }
        else if (op.type == FL_OPERATION_INSTR)
            fl_byte_vec_push(&cnt->byte_code, op.instr);
    }

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
        _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);
    }
    else if (id_chain->size == 1)
    {
        const fl_char *name = id_chain->data[0];
        // handle new id creation
        id = _fl_cmp_handle_new_id(name, cnt);

        // find new ids path
        fl_int_vec_free(&inds);
        inds = fl_scope_find_id_path(cnt->base->scope, id_chain, &offset);

        _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);
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

        assert(fl_operation_is_callable(op));

        fl_param_vec params = _fl_cmp_parse_nodes_to_params(id_node->next, cnt);
        const fl_fn_interface *fn_i = fl_operation_get_interface_p(op, params);

        assert(fn_i);

        fl_param_traits traits = FL_PARAM_TRAITS_NONE;

        if (_fl_cmp_is_call_const(op, fn_i, params))
            fl_bitfield_set(traits, FL_PARAM_TRAITS_CONST);

        fl_param_vec_free(&params);
        return fl_param_create_traited(fn_i->return_type, traits);
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_id_entry *id = fl_scope_resolve_id(cnt->base->scope, node->data);
        // do we want to allow use of undefined id chains?
        // (shoud an error be thrown?)
        return fl_param_create_traited(id ? id->value.data_type : FL_DATA_TYPE_NULL, FL_PARAM_TRAITS_REF);
    }
    case FL_AST_NODE_TYPE_LITERAL:
    {
        fl_value *lit_val = node->data;
        return fl_param_create_traited(lit_val->data_type, FL_PARAM_TRAITS_CONST);
    }
    case FL_AST_NODE_TYPE_ARRAY:
    {
        return fl_param_create(FL_DATA_TYPE_ARRAY);
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

inline fl_void _fl_cmp_byte_code_push_id_access_by_inds(fl_byte_vec *code, fl_uint offset, fl_int_vec inds)
{
    fl_byte_vec_push(code, FL_INSTR_GET_ID_BY_INDS);

    // push scope back offset
    fl_byte_vec_push_mem(code, &offset, sizeof(offset));
    // push size of indexes
    fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
    // push actual indexes
    fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);
}

fl_void _fl_cmp_compile_args(const fl_ast_node *arg_node, fl_cmp_context *cnt)
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
    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_NEW_ID);
    fl_byte_vec_push_str(&cnt->byte_code, name);

    // create dummy id in current scope
    fl_id_entry *id;
    fl_id_entry_init(&id, fl_str_dup(name), FL_DATA_TYPE_ANY, FL_VALUE_EMPTY, FL_FALSE);
    fl_id_entry_vec_push(&cnt->base->scope->ids, id);

    return id;
}

inline fl_void _fl_cmp_paste_byte_code_header(fl_byte_vec *bytes)
{
    fl_byte_vec_push(bytes, FL_SECTION_MARK_HEADER);

    // push byte code version
    // size + 1 for '\0' (end of string)
    fl_byte_vec_push_str(bytes, _BYTECODE_VERSION);

    // add compilation timestamp
    fl_long timestamp = fl_time_get_unix();
    fl_byte_vec_push_t(bytes, timestamp, fl_long);
}

fl_value _fl_cmp_parse_node_to_value(fl_ast_node *node, fl_cmp_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
    {
        // create new compilation context
        fl_cmp_context inner_cnt;
        fl_cmp_context_init(&inner_cnt, cnt->base);

        _fl_cmp_compile_block(node->child, &inner_cnt);

        fl_byte_vec *bytes = fl_mem_alloc_t(fl_byte_vec);
        *bytes = inner_cnt.byte_code;

        return fl_value_create(FL_DATA_TYPE_SCOPE, bytes);
    }
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_ast_node *id_node = node->child;
        fl_ast_node *inner_arg_node = id_node->data;
        fl_operation op = _fl_cmp_id_node_to_operation(id_node, cnt->base->scope);
        fl_value_vec args = _fl_cmp_eval_args(id_node->next, op, cnt);

        fl_value res = fl_operation_run(op, args, cnt->base);

        fl_value_vec_free(&args);
        return res;
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_id_chain *id_chain = node->data;
        fl_id_entry *id = fl_scope_resolve_id(cnt->base->scope, id_chain);

        // Allow undefined ids?

        if (!id && id_chain->size == 1)
            return fl_value_create(FL_DATA_TYPE_REF, _fl_cmp_handle_new_id(id_chain->data[0], cnt));
        else
            return id ? id->value : FL_VALUE_EMPTY;
    }
    case FL_AST_NODE_TYPE_LITERAL:
    {
        fl_value *lit_val = (fl_value *)node->data;
        return fl_value_copy(*lit_val);
    }
    case FL_AST_NODE_TYPE_ARRAY:
    {
        fl_value_vec *items = fl_mem_alloc_t(fl_value_vec);
        *items = _fl_cmp_parse_nodes_to_values(node->child, cnt);
        return fl_value_create(FL_DATA_TYPE_ARRAY, items);
    }
    default:
        assert(FL_FALSE);
        return FL_VALUE_EMPTY;
    }
}

inline fl_value_vec _fl_cmp_parse_nodes_to_values(fl_ast_node *arg_node, fl_cmp_context *cnt)
{
    fl_value_vec args;
    fl_value_vec_init(&args);

    while (arg_node)
    {
        fl_value val = _fl_cmp_parse_node_to_value(arg_node, cnt);
        fl_value_vec_push(&args, val);
        arg_node = arg_node->next;
    }

    return args;
}

inline fl_void _fl_cmp_byte_code_push_id_access_by_names(fl_byte_vec *code, const fl_id_chain *chain)
{
    fl_char buffer[FL_ID_CHAIN_MAX_SIZE];
    fl_id_chain_to_string(chain, buffer, FL_ID_CHAIN_MAX_SIZE);
    fl_byte_vec_push(code, FL_INSTR_GET_ID_BY_NAMES);
    fl_byte_vec_push_str(code, buffer);
}

fl_value_vec _fl_cmp_eval_args(fl_ast_node *arg_node, fl_operation op, fl_cmp_context *cnt)
{
    // handle lamda instruction diffrently
    if (op.instr == FL_INSTR_LAMBDA)
    {
        fl_value_vec args;
        fl_value_vec_init_c(&args, 2);

        fl_value lambda_head = _fl_cmp_parse_node_to_value(arg_node, cnt);

        // TODO: handle lambda paremeters here

        if (lambda_head.data_type != FL_DATA_TYPE_ARRAY)
        {
            // convert first arg to array
            fl_value_vec *arr = fl_mem_alloc_t(fl_value_vec);
            fl_value_vec_init_with(arr, 1, (fl_value[]){lambda_head});
            fl_value_vec_push(&args, fl_value_create(FL_DATA_TYPE_ARRAY, arr));
        }
        else
            fl_value_vec_push(&args, lambda_head);

        fl_value body;
        if (arg_node->next->type != FL_AST_NODE_TYPE_BLOCK)
        {
            // convert second arg to scope
            fl_cmp_context inner_cnt;
            fl_cmp_context_init(&inner_cnt, cnt->base);

            _fl_cmp_compile_node(arg_node->next, &inner_cnt);

            fl_byte_vec *bytes = fl_mem_alloc_t(fl_byte_vec);
            *bytes = inner_cnt.byte_code;

            body = fl_value_create(FL_DATA_TYPE_SCOPE, bytes);
        }
        else
            body = _fl_cmp_parse_node_to_value(arg_node->next, cnt);

        fl_value_vec_push(&args, body);

        return args;
    }

    return _fl_cmp_parse_nodes_to_values(arg_node, cnt);
}

inline fl_bool _fl_cmp_is_call_const(fl_operation op, const fl_fn_interface *intf, const fl_param_vec actual_params)
{
    return (fl_bitfield_is_set(intf->traits, FL_FN_TRAITS_PURE) &&
            fl_param_vec_is_every_const(&actual_params)) ||
           fl_operation_is_const_assign(op, actual_params.data[1]) ||
           op.instr == FL_INSTR_LAMBDA;
}

fl_bool _fl_cmp_try_op_inline(fl_ast_node *id_node, fl_operation op, fl_cmp_context *cnt)
{
    fl_ast_node *arg_node = id_node->next;
    fl_param_vec params = _fl_cmp_parse_nodes_to_params(arg_node, cnt);
    const fl_fn_interface *fn_i = fl_operation_get_interface_p(op, params);

    if (!fn_i)
    {
        throw_compilation_error(id_node, "couldn't find function overload for given arguments");
        return FL_FALSE;
    }

    fl_bool constant = _fl_cmp_is_call_const(op, fn_i, params);

    fl_param_vec_free(&params);

    if (!constant)
        return FL_FALSE;

    // simulate this operation

    fl_value_vec args = _fl_cmp_eval_args(arg_node, op, cnt);
    fl_value res = fl_operation_run(op, args, cnt->base);

    // never inline assign instruction, because id value will be erased after compilation
    if (!fl_operation_is_instr(op, FL_INSTR_ASSIGN))
    {
        // inline result

        if (fl_value_is_empty(res))
            assert(FL_FALSE);

        // push immediate value
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_IMMED_VALUE);
        fl_value_to_bytes(res, &cnt->byte_code);

        // not sure if its always good to free
        fl_value_free(&res);
    }

    return FL_TRUE;
}