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
#include "context/cmp_context.h"
#include "context/intpr_context.h"
#include "ast/ast_node.h"
#include "flow/operation.h"
#include "flow/id/id_chain.h"
#include "flow/byte_segm.h"
#include "vecs/cstr_vec.h"
#include "compiler/compiler.h"

#ifndef NDEBUG

#include "tools/byte_reader.h"
#include "tools/printer.h"

#endif

// ---------- DECLARATIONS ----------

/**
 * @brief Convert lambda AST nodes to bytes.
 * @param arg_node First lambda argument node.
 * @param cnt Context to use.
 */
fl_void _fl_inl_lambda_to_bytes(const fl_ast_node *arg_node, fl_cmp_context *cnt);

/**
 * @brief Guess node's resulting data descriptor.
 * @param node Node to examine.
 * @param cnt Context to use
 * @return Deduced data descriptor.
 */
fl_data_desc _fl_inl_guess_node_desc(const fl_ast_node *node, const fl_cmp_context *cnt);

/**
 * @brief Tell whether it's possible to inline operation.
 * @param op Operation to check.
 * @param intf Operation interface.
 * @param arg_descs Argument data descriptors
 * @return Boolean value whether operation is inlineable.
 */
fl_bool _fl_inl_is_call_inlineable(const fl_operation op, const fl_fn_intf *intf, const fl_data_desc_vec arg_descs);

/**
 * @brief Convert AST nodes to data descriptors vector describing them.
 * @param node First node to convert.
 * @param cnt Context to use.
 * @return Nodes data descriptors vector.
 */
FL_SEALED_FN fl_data_desc_vec _fl_inl_convert_nodes_to_descs(const fl_ast_node *node, const fl_cmp_context *cnt);

// ---------- DEFINITIONS -----------

fl_bool fl_inl_try_operation_inline(
    const fl_ast_node *id_node,
    const fl_operation op,
    fl_cmp_context *cnt)
{
    fl_ast_node *arg_node = id_node->next;

    // Retrieve operation data
    fl_data_desc_vec descs = _fl_inl_convert_nodes_to_descs(arg_node, cnt);
    const fl_fn_intf *fn_i = fl_operation_get_intf_by_descs(op, &descs);

    if (!fn_i)
    {
        __fl_throw_compilation_error(id_node, "couldn't find function overload for given arguments");
        return FL_FALSE;
    }

    fl_bool inlineable = _fl_inl_is_call_inlineable(op, fn_i, descs);
    fl_data_desc_vec_free(&descs);

    if (!inlineable)
        return FL_FALSE;

    // Ready to inline

    // Compile call to byte code
    fl_cmp_context inl_cnt;
    fl_cmp_context_from(&inl_cnt, (fl_local_context *)cnt);

    // Handle lambdas differently
    if (fl_operation_is_instr(op, FL_INSTR_LAMBDA))
        _fl_inl_lambda_to_bytes(arg_node, &inl_cnt);
    else
        fl_cmp_call_to_bytes(id_node, op, &inl_cnt);

    // Run this code snippet
    fl_byte_segm segm = fl_byte_segm_create_from_vec(&inl_cnt.byte_code);
    fl_value res = fl_intpr_eval(segm, (fl_local_context *)&inl_cnt);

    if (fl_value_is_empty(res))
        assert(FL_FALSE);

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_VALUE);
    fl_value_to_bytes(res, &cnt->byte_code);

    // Free resulting stuff
    fl_value_free(&res);
    fl_cmp_context_free(&inl_cnt);

    return FL_TRUE;
}

fl_void _fl_inl_lambda_to_bytes(const fl_ast_node *arg_node, fl_cmp_context *cnt)
{
    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_LAMBDA);

    const fl_ast_node *param_node = arg_node->child;
    fl_bool single_param = FL_FALSE;
    // Init array of labels (lambda parameter names)
    fl_cstr_vec labels;

    fl_cstr_vec_init(&labels);

    // Compile each label
    // For now the only information is parameter name

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
                assert(FL_FALSE);

            // Push label
            fl_cstr_vec_push(&labels, id_chain->data[0]);
        }
        else
        {
            fl_cmp_context inl_cnt;
            fl_cmp_context_from(&inl_cnt, (fl_local_context *)cnt);

            // Compile node normally and evaluate compilation
            fl_cmp_compile_node(param_node, &inl_cnt);

            fl_byte_segm segm = fl_byte_segm_create_from_vec(&inl_cnt.byte_code);
            fl_value res = fl_intpr_eval(segm, (fl_local_context *)&inl_cnt);

            // Resulting value has to be a string
            if (res.desc.type != FL_DATA_TYPE_STRING)
            {
                fl_value_free(&res);
                assert(FL_FALSE);
            }

            fl_cstr_vec_push(&labels, res.data);
            fl_cmp_context_free(&inl_cnt);
        }

        if (single_param)
            break;

        param_node = param_node->next;
    }

    // Convert array of strings to bytes
    fl_value lbl_arr = fl_value_from_cstr_vec(labels);

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_VALUE);
    fl_value_to_bytes(lbl_arr, &cnt->byte_code);

    // Compile lambda body
    const fl_ast_node *body_node = arg_node->next;

    // If lambda is a single operator, wrap it in a block
    fl_cmp_context block_cnt;
    fl_cmp_context_from(&block_cnt, (fl_local_context *)cnt);
    fl_local_context_deepen((fl_local_context *)&block_cnt);

    // Skip block handling, since we override it's default behaviour
    if (body_node->type == FL_AST_NODE_TYPE_BLOCK)
        body_node = body_node->child;

    while (body_node)
    {
        fl_cmp_compile_node(body_node, &block_cnt);
        body_node = body_node->next;
    }

    fl_cmp_block_cnt_to_bytes(&cnt->byte_code, &block_cnt, &labels);
    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_EXECUTE);

    // Free structs
    fl_value_free(&lbl_arr);
    fl_cstr_vec_free_shallow(&labels);
    fl_cmp_context_free(&block_cnt);
}

fl_bool _fl_inl_is_call_inlineable(const fl_operation op, const fl_fn_intf *intf, const fl_data_desc_vec arg_descs)
{
    return fl_operation_is_compile_time(op) ||
           (fl_bits_is_set(intf->traits, FL_FN_TRAITS_PURE) &&
            fl_data_desc_vec_is_every_const(&arg_descs));
}

FL_SEALED_FN fl_data_desc_vec _fl_inl_convert_nodes_to_descs(const fl_ast_node *node, const fl_cmp_context *cnt)
{
    fl_data_desc_vec descs;
    fl_data_desc_vec_init(&descs);

    while (node)
    {
        fl_data_desc desc = _fl_inl_guess_node_desc(node, cnt);
        fl_data_desc_vec_push(&descs, desc);
        node = node->next;
    }

    return descs;
}

fl_data_desc _fl_inl_guess_node_desc(const fl_ast_node *node, const fl_cmp_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        return fl_data_desc_create_traited(FL_DATA_TYPE_SCOPE | FL_DATA_TYPE_CODE_SEGM, FL_DATA_TRAIT_CONST);
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
    {
        fl_ast_node *id_node = node->child;
        fl_operation op = fl_operation_from_id_node(id_node, cnt->scope);

        if (op.type == FL_OPERATION_UNKNOWN)
            return fl_data_desc_create(FL_DATA_TYPE_ANY);

        assert(fl_operation_is_callable(op));

        // Get operation interface
        fl_data_desc_vec descs = _fl_inl_convert_nodes_to_descs(id_node->next, cnt);
        const fl_fn_intf *fn_i = fl_operation_get_intf_by_descs(op, &descs);

        assert(fn_i);

        // Set descriptor
        fl_data_traits traits = FL_DATA_TRAIT_NONE;

        // If we can predict result of the operation, it's inherently constant.
        if (_fl_inl_is_call_inlineable(op, fn_i, descs))
            fl_bits_set(traits, FL_DATA_TRAIT_CONST);

        fl_data_desc_vec_free(&descs);
        return fl_data_desc_create_traited(fn_i->return_type, traits);
    }
    case FL_AST_NODE_TYPE_ID_CHAIN:
    {
        fl_id_entry *id = fl_scope_find_id(cnt->scope, node->data);
        fl_data_type type = id ? id->decl_type : FL_DATA_TYPE_NULL;
        return fl_data_desc_create_traited(type, FL_DATA_TRAIT_REF);
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