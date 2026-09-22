/**
 * ---------- compiler.c ---------------
 *
 * Implements compiler logic.
 *
 * -------------------------------------
 */

#include "compiler.h"

// ---------- DEPENDENCIES -------------

#include "shared/error.h"
#include "ast/ast_node.h"
#include "shared/str.h"
#include "vecs/fn_variant_vec.h"
#include "ast/ast_node.h"
#include "flow/scope.h"
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

#ifndef NDEBUG

#include <stdio.h>

#endif

// ------------- MACROS -----------------

/**
 * @brief Shorthand for throwing a compilation error.
 */
#define __fl_throw_compilation_error(ast_node_ptr, message)                        \
    {                                                                              \
        fl_char buffer[FL_ID_CHAIN_MAX_SIZE];                                      \
        fl_id_chain_to_string(ast_node_ptr->data, buffer, FL_ID_CHAIN_MAX_SIZE);   \
        fl_err_throw(FL_ERR_COMPILATION, message, buffer, ast_node_ptr->location); \
    }

#ifndef NDEBUG

#define _FL_ID_STRING_MAX_SIZE 1024

#endif

// ---------- DECLARATIONS --------------

/**
 * @brief Compile block node.
 * @param node Node to compile.
 * @param cnt Context to use.
 */
fl_void _fl_cmp_compile_block(const fl_ast_node *node, fl_cmp_context *cnt);

/**
 * @brief Compile operation call node.
 * @param op_node Operation node to compile.
 * @param cnt Context to use.
 */
fl_void _fl_cmp_compile_op_call(const fl_ast_node *op_node, fl_cmp_context *cnt);

/**
 * @brief Compile id chain node.
 * @param node AST id node.
 * @param cnt Context to use.
 */
fl_void _fl_cmp_compile_id_chain(const fl_ast_node *node, fl_cmp_context *cnt);

/**
 * @brief Stamp id access in context bytes.
 * @param cnt Context to stamp to.
 * @param id_node Id node to use.
 */
FL_INLINE_FN fl_void _fl_cmp_stamp_id_access(fl_cmp_context *cnt, const fl_ast_node *id_node);

/**
 * @brief Compile literal node.
 * @param node Literal node to compile.
 * @param cnt Context to use.
 */
fl_void _fl_cmp_compile_literal(const fl_ast_node *node, fl_cmp_context *cnt);

/**
 * @brief Write id access by name (id chain) to byte code vector.
 * @param code Byte code vector to write to.
 * @param chain Id chain to convert.
 */
fl_void _fl_cmp_byte_code_push_id_access_by_name(fl_byte_vec *code, const fl_id_chain *chain);

/**
 * @brief Convert lambda AST nodes to bytes.
 * @param arg_node First lambda argument node.
 * @param cnt Context to use.
 */
fl_void _fl_cmp_lambda_args_to_bytes(const fl_ast_node *arg_node, fl_cmp_context *cnt);

/**
 * @brief Parse lambda parameter nodes.
 * @param param_node First parameter. May be single param or param array.
 * @param cnt Context to use.
 * @return Resulting labels string vector.
 */
fl_cstr_vec _fl_cmp_parse_lambda_params(const fl_ast_node *param_node, const fl_local_context *cnt);

/**
 * @brief Compiles operation call AST nodes.
 * @param id_node AST node of id of the operation.
 * @param op Operation to compile.
 * @param cnt Context to use.
 */
fl_void _fl_cmp_call_to_bytes(
    const fl_ast_node *id_node,
    const fl_operation op,
    fl_cmp_context *cnt);

/**
 * @brief Convert compilation context as a code block to bytes.
 * @param bytes Byte vector to write to.
 * @param cnt Context to convert.
 * @param ignore_labels Optional id labels to ignore when
 *                      declaring ids at the beginning of the block.
 */
fl_void _fl_cmp_block_cnt_to_bytes(
    fl_byte_vec *bytes,
    const fl_cmp_context *cnt,
    const fl_cstr_vec *ignore_labels);

/**
 * @brief Compile node with guarantee that it'll be in a block.
 * @param node AST node to compile.
 * @param cnt Context to use.
 */
fl_void _fl_cmp_compile_as_block_wrapped(const fl_ast_node *node, fl_cmp_context *cnt);

/**
 * @brief Compile single AST node.
 * @param node Node to compile.
 * @param cnt Context to use while compilation.
 */
fl_void _fl_cmp_compile_node(const fl_ast_node *node, fl_cmp_context *cnt);

/**
 * @brief Handle new identifier.
 * @param name Identifier name.
 * @param cnt Context to use.
 * @return New id entry.
 */
FL_INLINE_FN fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt);

/**
 * @brief Stamp byte code header in byte vector.
 * @param bytes Byte vector to stamp to.
 */
FL_INLINE_FN fl_void _fl_cmp_paste_byte_code_header(fl_byte_vec *bytes);

/**
 * @brief Create literal from value
 * @param lit_value Value of literal.
 * @param cnt Context to use.
 * @return Index of created literal in context literal table.
 */
FL_INLINE_FN fl_size _fl_cmp_make_literal(const fl_value lit_value, fl_cmp_context *cnt);

// ---------- DEFINITIONS --------------

fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *tree)
{
    fl_byte_vec compiled;
    fl_base_context base;
    fl_cmp_context cnt;

    // Init stuff
    fl_base_context_init(&base);
    fl_cmp_context_init(&cnt, &base);
    fl_byte_vec_init(&compiled);

    // Compile nodes
    _fl_cmp_compile_node(tree, &cnt);

    // Add byte code version info
    _fl_cmp_paste_byte_code_header(&compiled);

    if (!fl_value_vec_is_empty(&cnt.base->literals))
    {
        // Add literals table
        const fl_size n = cnt.base->literals.size;

        fl_byte_vec_push(&compiled, FL_SECTION_MARK_LITERALS);
        fl_byte_vec_push_t(&compiled, n, fl_size);

        for (fl_size i = 0; i < n; i++)
            fl_value_to_bytes(cnt.base->literals.data[i], &compiled);
    }

    // Merge both byte vecs
    fl_byte_vec_push(&compiled, FL_SECTION_MARK_CODE);
    fl_byte_vec_merge(&compiled, &cnt.byte_code);

    // Free remains
    fl_cmp_context_free(&cnt);
    fl_base_context_free(&base);

    return compiled;
}

fl_void _fl_cmp_compile_node(const fl_ast_node *node, fl_cmp_context *cnt)
{
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        _fl_cmp_compile_block(node, cnt);
        break;
    case FL_AST_NODE_TYPE_FN_CALL:
    case FL_AST_NODE_TYPE_OPERATOR:
        // Fn call and operator are generalized to operation struct
        _fl_cmp_compile_op_call(node, cnt);
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
        _fl_cmp_compile_id_chain(node, cnt);
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        _fl_cmp_compile_literal(node, cnt);
        break;
    default:
        assert(FL_FALSE);
        break;
    }
}

fl_void _fl_cmp_compile_block(const fl_ast_node *node, fl_cmp_context *cnt)
{
    fl_cmp_context inner_cnt;

    // Init context
    fl_cmp_context_from(&inner_cnt, (fl_local_context *)cnt);
    fl_local_context_deepen((fl_local_context *)&inner_cnt);

    node = node->child;
    while (node)
    {
        _fl_cmp_compile_node(node, &inner_cnt);
        node = node->next;
    }

    // Convert whole context to bytes
    _fl_cmp_block_cnt_to_bytes(&cnt->byte_code, &inner_cnt, FL_NULL);

    // Free context
    fl_local_context_free_scope((fl_local_context *)&inner_cnt);
    fl_cmp_context_free(&inner_cnt);
}

fl_void _fl_cmp_compile_op_call(const fl_ast_node *op_node, fl_cmp_context *cnt)
{
    fl_ast_node *id_node = op_node->child;

    fl_operation op = fl_operation_from_id_node(id_node, cnt->scope);

    if (op.type == FL_OPERATION_UNKNOWN)
    {
        // Just compile it since we can't opt this out
        _fl_cmp_call_to_bytes(id_node, op, cnt);

#if _INCLUDE_INLINE_LOGS
        goto __return;
#else
        return;
#endif
    }

    fl_data_desc_vec descs = fl_ast_nodes_get_desc_vec(id_node->next, (fl_local_context *)cnt);
    const fl_fn_intf *fn_i = fl_operation_get_intf_by_descs(op, &descs);

    if (!fn_i)
    {
        __fl_throw_compilation_error(id_node, "couldn't find function overload for given arguments");
        assert(FL_FALSE);
    }

    fl_cmp_context inl_cnt;

    fl_bool inlineable = fl_fn_intf_is_inlineable(fn_i, &descs);
    fl_data_desc_vec_free(&descs);

    fl_cmp_context *curr_cnt = cnt;

    if (inlineable)
    {
        fl_cmp_context_from(&inl_cnt, (fl_local_context *)cnt);
        // Write to internal context if we are going to inline this
        curr_cnt = &inl_cnt;
    }

    if (!fl_operation_is_callable(op))
        assert(FL_FALSE);

    // Compile it normally
    _fl_cmp_call_to_bytes(id_node, op, curr_cnt);

    // Inline if possible
    if (inlineable)
    {
        // Run this code snippet
        fl_byte_segm segm = fl_byte_segm_create_from_vec(&curr_cnt->byte_code);
        fl_value res = fl_intpr_eval(segm, (fl_local_context *)curr_cnt);

        if (fl_value_is_empty(res))
            assert(FL_FALSE);

        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_VALUE);
        fl_value_to_bytes(res, &cnt->byte_code);

        // Free resulting value after converting it to byte code
        fl_value_free(&res);
        // Free internal context
        fl_cmp_context_free(&inl_cnt);
    }

#if _INCLUDE_INLINE_LOGS
__return:

    {
        const fl_size line = id_node->location.line + 1;
        fl_char buffer[_FL_ID_STRING_MAX_SIZE];
        assert(fl_id_chain_to_string(id_node->data, buffer, _FL_ID_STRING_MAX_SIZE));
        printf("!!! COMPILER !!! Passed operation '%s', line %u\n", buffer, line);
    }
#endif
}

FL_INLINE_FN fl_void _fl_cmp_stamp_id_access(fl_cmp_context *cnt, const fl_ast_node *id_node)
{
    fl_char buffer[FL_ID_CHAIN_MAX_SIZE];
    fl_id_chain *id_chain = id_node->data;

    fl_id_chain_to_string(id_chain, buffer, FL_ID_CHAIN_MAX_SIZE);
    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_GET_ID);
    fl_byte_vec_push_str(&cnt->byte_code, buffer);
}

FL_INLINE_FN fl_size _fl_cmp_make_literal(const fl_value lit_value, fl_cmp_context *cnt)
{
    fl_value_vec *lits = &cnt->base->literals;
    fl_int ind = fl_value_vec_find_identical(lits, lit_value);

    if (ind == FL_FIND_FAIL)
    {
        fl_value_vec_push(lits, fl_value_copy(lit_value));
        return lits->size - 1;
    }

    return (fl_size)ind;
}

fl_void _fl_cmp_compile_literal(const fl_ast_node *node, fl_cmp_context *cnt)
{
    fl_value *value = node->data;
    fl_size index = _fl_cmp_make_literal(*value, cnt);

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_GET_LITERAL);
    fl_byte_vec_push_mem(&cnt->byte_code, &index, sizeof(index));
}

fl_void _fl_cmp_compile_id_chain(const fl_ast_node *node, fl_cmp_context *cnt)
{
    fl_id_chain *id_chain = node->data;

    // Try to find this id
    fl_id_entry *id = fl_scope_find_id(cnt->scope, id_chain);

    if (!id && id_chain->size == 1)
        // Create new id in current scope if accessor isn't chained
        id = _fl_cmp_handle_new_id(id_chain->data[0], cnt);

    // Stamp id access even if id is unrecognized at this stage
    _fl_cmp_stamp_id_access(cnt, node);
}

FL_INLINE_FN fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt)
{
    // Create dummy id in base scope
    fl_id_entry *id;
    fl_id_entry_init(&id, fl_str_dup(name), FL_DATA_TYPE_ANY, fl_value_empty());
    fl_id_entry_vec_push(&cnt->scope->ids, id);

    return id;
}

FL_INLINE_FN fl_void _fl_cmp_paste_byte_code_header(fl_byte_vec *bytes)
{
    fl_byte_vec_push(bytes, FL_SECTION_MARK_HEADER);

    // Push byte code version
    fl_byte_vec_push_str(bytes, _BYTECODE_VERSION);

    // Add compilation timestamp
    fl_long timestamp = fl_time_get_unix();
    fl_byte_vec_push_t(bytes, timestamp, fl_long);
}

fl_void _fl_cmp_byte_code_push_id_access_by_name(fl_byte_vec *code, const fl_id_chain *chain)
{
    fl_char buffer[FL_ID_CHAIN_MAX_SIZE];
    fl_id_chain_to_string(chain, buffer, FL_ID_CHAIN_MAX_SIZE);
    fl_byte_vec_push(code, FL_INSTR_GET_ID);
    fl_byte_vec_push_str(code, buffer);
}

fl_void _fl_cmp_call_to_bytes(
    const fl_ast_node *id_node,
    const fl_operation op,
    fl_cmp_context *cnt)
{
    fl_ast_node *arg_node = id_node->next;
    fl_bool compiled = FL_FALSE;

    if (op.type == FL_OPERATION_INSTR)
    {
        fl_byte_vec_push(&cnt->byte_code, op.instr);

        // Compile some instructions differently

        switch (op.instr)
        {
        case FL_INSTR_LAMBDA:
            _fl_cmp_lambda_args_to_bytes(arg_node, cnt);
            compiled = FL_TRUE;
            break;
        case FL_INSTR_WHILE:
            _fl_cmp_compile_as_block_wrapped(arg_node, cnt);
            _fl_cmp_compile_as_block_wrapped(arg_node->next, cnt);

            assert(arg_node->next->next == FL_NULL);
            compiled = FL_TRUE;
            break;
        case FL_INSTR_IF:
        case FL_INSTR_ELSE:
            _fl_cmp_compile_node(arg_node, cnt);
            _fl_cmp_compile_as_block_wrapped(arg_node->next, cnt);

            assert(arg_node->next->next == FL_NULL);
            compiled = FL_TRUE;
            break;
        }
    }
    else if (fl_bits_is_any(op.type, FL_OPERATION_FN | FL_OPERATION_UNKNOWN))
    {
        // Start CALL instruction
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_CALL);
        // Push information how to access id
        _fl_cmp_stamp_id_access(cnt, id_node);
    }
    else
        assert(FL_FALSE);

    if (!compiled)
    {
        // Compile args

        while (arg_node)
        {
            _fl_cmp_compile_node(arg_node, cnt);
            arg_node = arg_node->next;
        }
    }

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_EXECUTE);
}

fl_void _fl_cmp_block_cnt_to_bytes(
    fl_byte_vec *bytes,
    const fl_cmp_context *cnt,
    const fl_cstr_vec *ignore_labels)
{
    fl_id_entry_vec ids = cnt->scope->ids;
    fl_size ignore_count = ignore_labels ? ignore_labels->size : 0;

    // Sum id declarations mem block size and rest of this block size
    fl_size total_size = cnt->byte_code.size;
    if (ids.size > ignore_count)
        // ids.size * 2 for each FL_INSTR_DECL_ID tag and string termination '\0'
        total_size += (ids.size - ignore_count) * 2;

    fl_byte_vec_push(bytes, FL_INSTR_START_BLOCK);
    // Push block byte size at the beginning and fill it later
    fl_size size_ind = bytes->size;
    fl_byte_vec_push_t(bytes, total_size, fl_size);

    // Push id declarations at block start
    for (fl_size i = 0; i < ids.size; i++)
    {
        const fl_char *name = ids.data[i]->name;
        if (ignore_count && fl_cstr_vec_find(ignore_labels, name) != FL_FIND_FAIL)
            continue;

        total_size += fl_str_len(name);
        fl_byte_vec_push(bytes, FL_INSTR_DECL_ID);
        fl_byte_vec_push_str(bytes, ids.data[i]->name);
    }

    // Set final block size
    *(fl_size *)&bytes->data[size_ind] = total_size;

    fl_byte_vec_merge(bytes, &cnt->byte_code);
    fl_byte_vec_push(bytes, FL_INSTR_END_BLOCK);
}

fl_cstr_vec _fl_cmp_parse_lambda_params(const fl_ast_node *param_node, const fl_local_context *cnt)
{
    fl_bool single_param = FL_FALSE;
    // Init array of labels (lambda parameter names)
    fl_cstr_vec labels;
    fl_cstr_vec_init(&labels);

    switch (param_node->type)
    {
    case FL_AST_NODE_TYPE_ID_CHAIN:
        single_param = FL_TRUE;
        break;
    case FL_AST_NODE_TYPE_ARRAY:
        param_node = param_node->child;
        break;
    default:
        assert(FL_FALSE);
        break;
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
            fl_cmp_context_from(&inl_cnt, cnt);

            // Compile node normally and evaluate compilation
            _fl_cmp_compile_node(param_node, &inl_cnt);

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

    return labels;
}

fl_void _fl_cmp_lambda_args_to_bytes(const fl_ast_node *arg_node, fl_cmp_context *cnt)
{
    fl_cstr_vec labels = _fl_cmp_parse_lambda_params(arg_node, (fl_local_context *)cnt);

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
        _fl_cmp_compile_node(body_node, &block_cnt);
        body_node = body_node->next;
    }

    _fl_cmp_block_cnt_to_bytes(&cnt->byte_code, &block_cnt, &labels);

    // Free structs
    fl_value_free(&lbl_arr);
    fl_cstr_vec_free_shallow(&labels);
    fl_cmp_context_free(&block_cnt);
}

fl_void _fl_cmp_compile_as_block_wrapped(const fl_ast_node *node, fl_cmp_context *cnt)
{
    if (node->type == FL_AST_NODE_TYPE_BLOCK)
        _fl_cmp_compile_node(node, cnt);
    else
    {
        fl_byte_vec *bytes = &cnt->byte_code;
        fl_size size = 0;

        fl_byte_vec_push(bytes, FL_INSTR_START_BLOCK);

        fl_size size_ind = bytes->size;
        // Push block size and fill it later
        fl_byte_vec_push_t(bytes, size, fl_size);
        size = cnt->byte_code.size;

        _fl_cmp_compile_node(node, cnt);
        *(fl_size *)&bytes->data[size_ind] = bytes->size - size;

        fl_byte_vec_push(bytes, FL_INSTR_END_BLOCK);
    }
}