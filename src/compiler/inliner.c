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

// ---------- DECLARATIONS ----------

fl_void _fl_inl_lambda_to_bytes(const fl_ast_node *arg_node, fl_cmp_context *cnt);
fl_data_desc _fl_inl_parse_node_to_param(const fl_ast_node *node, const fl_cmp_context *cnt);
inline fl_bool _fl_inl_is_call_inlineable(fl_operation op, const fl_fn_interface *intf, const fl_data_desc_vec actual_params);
inline fl_data_desc_vec _fl_inl_parse_nodes_to_params(const fl_ast_node *arg_node, const fl_cmp_context *cnt);

// ---------- DEFINITIONS -----------

fl_bool fl_inl_try_op_inline(fl_ast_node *id_node, fl_operation op, fl_cmp_context *cnt)
{
    fl_ast_node *arg_node = id_node->next;
    fl_data_desc_vec params = _fl_inl_parse_nodes_to_params(arg_node, cnt);
    const fl_fn_interface *fn_i = fl_operation_get_interface(op, params);

    if (!fn_i)
    {
        throw_compilation_error(id_node, "couldn't find function overload for given arguments");
        return FL_FALSE;
    }

    fl_bool inlineable = _fl_inl_is_call_inlineable(op, fn_i, params);
    fl_data_desc_vec_free(&params);

    if (!inlineable)
        return FL_FALSE;

    // ready to inline

    // compile call to byte code
    fl_cmp_context inl_cnt;
    fl_cmp_context_from(&inl_cnt, (fl_local_context *)cnt);

    // handle lambdas differently
    if (fl_operation_is_instr(op, FL_INSTR_LAMBDA))
        _fl_inl_lambda_to_bytes(arg_node, &inl_cnt);
    else
        fl_cmp_call_to_bytes(&inl_cnt, id_node, op);

#ifndef NDEBUG

    fl_byte_reader reader = fl_byte_reader_create(inl_cnt.byte_code);
    fl_prt_print_byte_code_code(&reader);

#endif

    // run this code snippet
    fl_value res = fl_intpr_eval(&inl_cnt);

    assert(!fl_value_is_empty(res));

    fl_value_to_bytes(res, &cnt->byte_code);

    // not sure if its always good to free resulting value
    fl_value_free(&res);

    fl_cmp_context_free(&inl_cnt);
    return FL_TRUE;
}

fl_void _fl_inl_lambda_to_bytes(const fl_ast_node *arg_node, fl_cmp_context *cnt)
{
    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_LAMBDA);

    // init array of labels (lambda parameter names)
    fl_value_vec *labels = fl_mem_alloc_t(fl_value_vec);
    const fl_ast_node *param_node = arg_node->child;
    fl_bool single_param = FL_FALSE;

    fl_value_vec_init(labels);

    // compile each potential label - all we want for now is a string

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
            fl_cmp_context inl_cnt;
            fl_cmp_context_from(&inl_cnt, (fl_local_context *)cnt);

            // compile node normally and eval compilation
            fl_cmp_compile_node(param_node, &inl_cnt);
            fl_value res = fl_intpr_eval(&inl_cnt);

            // resulting value has to be a string
            // (can it be ref too?)
            if (res.desc.type != FL_DATA_TYPE_STRING)
            {
                fl_value_free(&res);
                assert(FL_FALSE);
            }

            fl_value_vec_push(labels, fl_value_create(FL_DATA_TYPE_STRING, res.data));
            fl_cmp_context_free(&inl_cnt);
        }

        if (single_param)
            break;

        param_node = param_node->next;
    }

    // convert array of strings to bytes
    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_VALUE);
    fl_value_to_bytes(fl_value_create(FL_DATA_TYPE_ARRAY, labels), &cnt->byte_code);

    // free labels when they are already copied to the bc
    fl_value_vec_free(labels);

    // compile lambda body

    if (arg_node->next->type == FL_AST_NODE_TYPE_BLOCK)
        fl_cmp_compile_node(arg_node->next, cnt);
    else
    {
        // simulate block if it's single operation lambda body

        fl_cmp_context block_cnt;
        fl_cmp_context_from(&block_cnt, (fl_local_context *)cnt);

        fl_cmp_compile_node(arg_node->next, &block_cnt);

        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_START_BLOCK);
        fl_byte_vec_push_t(&cnt->byte_code, block_cnt.byte_code.size, fl_size);
        fl_byte_vec_merge(&cnt->byte_code, &block_cnt.byte_code);
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_END_BLOCK);

        fl_cmp_context_free(&block_cnt);
    }

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_END_ARGS);
}

inline fl_bool _fl_inl_is_call_inlineable(fl_operation op, const fl_fn_interface *intf, const fl_data_desc_vec actual_params)
{
    return fl_operation_is_compile_time(op) ||
           (fl_bitfield_is_set(intf->traits, FL_FN_TRAITS_PURE) &&
            fl_data_desc_vec_is_every_const(&actual_params)) ||
           fl_operation_is_const_assign(op, actual_params.data[1]);
}

inline fl_data_desc_vec _fl_inl_parse_nodes_to_params(const fl_ast_node *arg_node, const fl_cmp_context *cnt)
{
    fl_data_desc_vec params;
    fl_data_desc_vec_init(&params);

    while (arg_node)
    {
        fl_data_desc par = _fl_inl_parse_node_to_param(arg_node, cnt);
        fl_data_desc_vec_push(&params, par);
        arg_node = arg_node->next;
    }

    return params;
}

fl_data_desc _fl_inl_parse_node_to_param(const fl_ast_node *node, const fl_cmp_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        return fl_data_desc_create_traited(FL_DATA_TYPE_SCOPE, FL_DATA_TRAITS_CONST);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_ast_node *id_node = node->child;
        fl_operation op = fl_operation_from_id_node(id_node, cnt->scope);

        assert(fl_operation_is_callable(op));

        fl_data_desc_vec params = _fl_inl_parse_nodes_to_params(id_node->next, cnt);
        const fl_fn_interface *fn_i = fl_operation_get_interface(op, params);

        assert(fn_i);

        fl_data_traits traits = FL_DATA_TRAITS_NONE;

        if (_fl_inl_is_call_inlineable(op, fn_i, params))
            fl_bitfield_set(traits, FL_DATA_TRAITS_CONST);

        fl_data_desc_vec_free(&params);
        return fl_data_desc_create_traited(fn_i->return_type, traits);
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_id_entry *id = fl_scope_resolve_id(cnt->scope, node->data);
        // do we want to allow use of undefined id chains?
        // (shoud an error be thrown?)
        return fl_data_desc_create_traited(id ? id->value.desc.type : FL_DATA_TYPE_NULL, FL_DATA_TRAITS_REF);
    }
    case FL_AST_NODE_TYPE_LITERAL:
    {
        fl_value *lit_val = node->data;
        return lit_val->desc;
    }
    case FL_AST_NODE_TYPE_ARRAY:
        return fl_data_desc_create(FL_DATA_TYPE_ARRAY);
    default:
        assert(FL_FALSE);
        return fl_data_desc_create(FL_DATA_TYPE_NULL);
    }
}