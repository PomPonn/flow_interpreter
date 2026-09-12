/**
 * ---------- inliner.c ----------------
 *
 * Implements inliner logic.
 *
 * -------------------------------------
 */

#include "inliner.h"

// ---------- DEPENDENCIES ----------

#include "interpreter/interpreter.h"
#include "compiler/compiler.h"
#include "ast/ast_node.h"
#include "flow/operation.h"
#include "context/cmp_context.h"
#include "context/intpr_context.h"
#include "flow/id/id_chain.h"
#include "compiler/shared.h"

#ifndef NDEBUG

#include "tools/byte_reader.h"
#include "tools/printer.h"

#endif

// ---------- MACROS ----------------

// ...

// ---------- GLOBALS ---------------

// ...

// ---------- DECLARATIONS ----------

fl_param _fl_inl_parse_node_to_param(const fl_ast_node *node, const fl_cmp_context *cnt);

fl_operation _fl_inl_id_node_to_operation(const fl_ast_node *id_node, const fl_scope *scope);

inline fl_bool _fl_inl_is_call_inlineable(fl_operation op, const fl_fn_interface *intf, const fl_param_vec actual_params);

inline fl_param_vec _fl_inl_parse_nodes_to_params(const fl_ast_node *arg_node, const fl_cmp_context *cnt);

fl_void _fl_inl_lambda_to_bytes(const fl_ast_node *arg_node, fl_cmp_context *cnt);

// ---------- DEFINITIONS -----------

fl_bool fl_inl_try_op_inline(fl_ast_node *id_node, fl_operation op, fl_cmp_context *cnt)
{
    fl_ast_node *arg_node = id_node->next;
    fl_param_vec params = _fl_inl_parse_nodes_to_params(arg_node, cnt);
    const fl_fn_interface *fn_i = fl_operation_get_interface_p(op, params);

    if (!fn_i)
    {
        throw_compilation_error(id_node, "couldn't find function overload for given arguments");
        return FL_FALSE;
    }

    fl_bool inlineable = _fl_inl_is_call_inlineable(op, fn_i, params);
    fl_param_vec_free(&params);

    if (!inlineable)
        return FL_FALSE;

    // ready to inline

    // handle lambdas differently
    if (op.type == FL_OPERATION_INSTR && op.instr == FL_INSTR_LAMBDA)
    {
        _fl_inl_lambda_to_bytes(arg_node, cnt);
        return FL_TRUE;
    }

    // compile call to byte code
    fl_cmp_context inl_cnt;
    fl_cmp_context_from(&inl_cnt, (fl_local_context *)cnt);

    fl_cmp_call_to_bytes(&inl_cnt, id_node, op);

    // run compilation
    fl_intpr_context intpr_cnt;
    fl_intpr_context_from(&intpr_cnt, (fl_local_context *)cnt);

#ifndef NDEBUG

    fl_byte_reader reader = fl_byte_reader_create(inl_cnt.byte_code);
    fl_prt_print_byte_code_code(&reader);

#endif

    fl_value res = fl_intpr_eval(inl_cnt.byte_code, &intpr_cnt);

    assert(!fl_value_is_empty(res));

    fl_value_to_bytes(res, &cnt->byte_code);

    // not sure if its always good to free resulting value
    fl_value_free(&res);

    fl_cmp_context_free(&inl_cnt);
    fl_intpr_context_free(&intpr_cnt);

    return FL_TRUE;
}

fl_void _fl_inl_lambda_to_bytes(const fl_ast_node *arg_node, fl_cmp_context *cnt)
{
    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_LAMBDA);

    // init array of labels (lambda parameter names)
    fl_value_vec *labels = fl_mem_alloc_t(fl_value_vec);
    fl_cmp_context inl_cnt;
    fl_intpr_context intpr_cnt;
    fl_bool single_param = FL_FALSE;
    const fl_ast_node *param_node = arg_node->child;

    fl_value_vec_init(labels);
    fl_cmp_context_from(&inl_cnt, (fl_local_context *)cnt);
    fl_intpr_context_from(&intpr_cnt, (fl_local_context *)cnt);

    // how to make context state not mutated after this?
    // compile each potential label - all we want is a string

    if (arg_node->type == FL_AST_NODE_TYPE_ID_CHAIN)
    {
        single_param = FL_TRUE;
        param_node = arg_node;
    }

    while (param_node)
    {
        if (param_node->type == FL_AST_NODE_TYPE_ID_CHAIN)
        {
            fl_id_chain *id_chain = param_node->data;

            if (id_chain->size != 1)
                // error
                assert(FL_FALSE);

            // push label
            fl_value_vec_push(labels, fl_value_create(FL_DATA_TYPE_STRING, id_chain->data[0]));
        }
        else
        {
            // compile node normally and eval compilation
            fl_cmp_compile_node(param_node, &inl_cnt);
            fl_value res = fl_intpr_eval(inl_cnt.byte_code, &intpr_cnt);

            // resulting value has to be a string
            // (can it be ref too?)
            if (res.data_type != FL_DATA_TYPE_STRING)
            {
                fl_value_free(&res);
                assert(FL_FALSE);
            }

            fl_value_vec_push(labels, fl_value_create(FL_DATA_TYPE_STRING, res.data));
        }

        if (single_param)
            break;

        param_node = param_node->next;
    }

    fl_cmp_context_free(&inl_cnt);
    fl_intpr_context_free(&intpr_cnt);

    // convert array of strings to bytes
    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_VALUE);
    fl_value_to_bytes(fl_value_create(FL_DATA_TYPE_ARRAY, labels), &cnt->byte_code);

    // free labels when they are already copied to the bc
    fl_value_vec_free(labels);

    // parse lambda body
    fl_cmp_compile_node(arg_node->next, cnt);
}

inline fl_bool _fl_inl_is_call_inlineable(fl_operation op, const fl_fn_interface *intf, const fl_param_vec actual_params)
{
    return fl_operation_is_compile_time(op) ||
           (fl_bitfield_is_set(intf->traits, FL_FN_TRAITS_PURE) &&
            fl_param_vec_is_every_const(&actual_params)) ||
           fl_operation_is_const_assign(op, actual_params.data[1]);
}

inline fl_param_vec _fl_inl_parse_nodes_to_params(const fl_ast_node *arg_node, const fl_cmp_context *cnt)
{
    fl_param_vec params;
    fl_param_vec_init(&params);

    while (arg_node)
    {
        fl_param par = _fl_inl_parse_node_to_param(arg_node, cnt);
        fl_param_vec_push(&params, par);
        arg_node = arg_node->next;
    }

    return params;
}

fl_operation _fl_inl_id_node_to_operation(const fl_ast_node *id_node, const fl_scope *scope)
{
    fl_id_chain *id_chain = id_node->data;

    // check if it's instruction
    fl_instruction instr = id_chain->size == 1 ? fl_instruction_from_alias(id_chain->data[0]) : FL_INSTR_NONE;
    if (instr)
        return fl_operation_from_instr(instr);

    // try to find function id
    fl_id_entry *id = fl_scope_resolve_id(scope, id_chain);

    if (id)
    {
        fl_value id_val = id->value;

        if (id_val.data_type == FL_DATA_TYPE_FN)
            // error (?) - not-a-function id used as a callable
            assert(FL_FALSE);

        return fl_operation_from_fn_vars(id_val.data);
    }

    return fl_operation_unknown();
}

fl_param _fl_inl_parse_node_to_param(const fl_ast_node *node, const fl_cmp_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        return fl_param_create_traited(FL_DATA_TYPE_SCOPE, FL_PARAM_TRAITS_CONST);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_ast_node *id_node = node->child;
        fl_operation op = _fl_inl_id_node_to_operation(id_node, cnt->scope);

        assert(fl_operation_is_callable(op));

        fl_param_vec params = _fl_inl_parse_nodes_to_params(id_node->next, cnt);
        const fl_fn_interface *fn_i = fl_operation_get_interface_p(op, params);

        assert(fn_i);

        fl_param_traits traits = FL_PARAM_TRAITS_NONE;

        if (_fl_inl_is_call_inlineable(op, fn_i, params))
            fl_bitfield_set(traits, FL_PARAM_TRAITS_CONST);

        fl_param_vec_free(&params);
        return fl_param_create_traited(fn_i->return_type, traits);
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_id_entry *id = fl_scope_resolve_id(cnt->scope, node->data);
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