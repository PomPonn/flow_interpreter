/**
 * ---------- compiler.c ---------------
 *
 * Implements compiler logic.
 *
 * -------------------------------------
 */

#include "compiler.h"

// ---------- DEPENDENCIES -------------

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
#include "compiler/inliner.h"
#include "tools/time.h"

#ifndef NDEBUG

#include <stdio.h>

#endif

// ------------- MACROS -----------------

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
fl_void _fl_cmp_stamp_id_access(fl_cmp_context *cnt, const fl_ast_node *id_node);

/**
 * @brief Compile literal node.
 * @param node Literal node to compile.
 * @param cnt Context to use.
 */
fl_void _fl_cmp_compile_literal(const fl_ast_node *node, fl_cmp_context *cnt);

/**
 * @brief Write id access by indices to byte code vector.
 * @param code Byte code vector to write to.
 * @param offset Id access offset.
 * @param inds Id access indices.
 */
fl_void _fl_cmp_byte_code_push_id_access_by_inds(fl_byte_vec *code, fl_uint offset, const fl_int_vec inds);

/**
 * @brief Write id access by name (id chain) to byte code vector.
 * @param code Byte code vector to write to.
 * @param chain Id chain to convert.
 */
fl_void _fl_cmp_byte_code_push_id_access_by_name(fl_byte_vec *code, const fl_id_chain *chain);

/**
 * @brief Handle new identifier.
 * @param name Identifier name.
 * @param cnt Context to use.
 * @return New id entry.
 */
FL_SEALED_FN fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt);

/**
 * @brief Stamp byte code header in byte vector.
 * @param bytes Byte vector to stamp to.
 */
FL_SEALED_FN fl_void _fl_cmp_paste_byte_code_header(fl_byte_vec *bytes);

/**
 * @brief Create literal from value
 * @param lit_value Value of literal.
 * @param cnt Context to use.
 * @return Index of created literal in context literal table.
 */
FL_SEALED_FN fl_size _fl_cmp_make_literal(const fl_value lit_value, fl_cmp_context *cnt);

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
    fl_cmp_compile_node(tree, &cnt);

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

fl_void fl_cmp_compile_node(const fl_ast_node *node, fl_cmp_context *cnt)
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
        fl_cmp_compile_node(node, &inner_cnt);
        node = node->next;
    }

    // Convert whole context to bytes
    fl_cmp_block_cnt_to_bytes(&cnt->byte_code, &inner_cnt, FL_NULL);

    // Free context
    fl_local_context_free_scope((fl_local_context *)&inner_cnt);
    fl_cmp_context_free(&inner_cnt);
}

fl_void _fl_cmp_compile_op_call(const fl_ast_node *op_node, fl_cmp_context *cnt)
{
    fl_ast_node *id_node = op_node->child;
    fl_operation op = fl_operation_from_id_node(id_node, cnt->scope);

    if (op.type != FL_OPERATION_UNKNOWN)
    {
        if (!fl_operation_is_callable(op))
            assert(FL_FALSE);

        // Try to inline first
        if (fl_inl_try_operation_inline(id_node, op, cnt))
        {
#if _INCLUDE_INLINE_LOGS
            goto __return;
#else
            return;
#endif
        }

        // Throw an error if compile-time only function wasn't inlined
        if (fl_operation_is_compile_time(op))
            assert(FL_FALSE);
    }

    // Compile it normally
    fl_cmp_call_to_bytes(id_node, op, cnt);

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

fl_void _fl_cmp_stamp_id_access(fl_cmp_context *cnt, const fl_ast_node *id_node)
{
    fl_uint offset = 0;
    fl_id_chain *id_chain = id_node->data;
    fl_int_vec inds = fl_scope_find_id_path(cnt->scope, id_chain, &offset);

    if (fl_int_vec_is_empty(&inds))
        _fl_cmp_byte_code_push_id_access_by_name(&cnt->byte_code, id_chain);
    else
        _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);

    fl_int_vec_free(&inds);
}

FL_SEALED_FN fl_size _fl_cmp_make_literal(const fl_value lit_value, fl_cmp_context *cnt)
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
    fl_id_chain *id_chain = (fl_id_chain *)node->data;

    // Try to find this id
    fl_uint offset;
    fl_int_vec inds = fl_scope_find_id_path(cnt->scope, id_chain, &offset);
    fl_id_entry *id = fl_scope_traverse(cnt->scope, inds, offset);

    // Push opted info how to to access this id if it's found (may be to change)
    if (id)
        _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);

    // Create new id if accessor isn't chained
    else if (id_chain->size == 1)
    {
        const fl_char *name = id_chain->data[0];
        // handle new id creation
        id = _fl_cmp_handle_new_id(name, cnt);

        // find new ids path
        fl_int_vec_free(&inds);
        inds = fl_scope_find_id_path(cnt->scope, id_chain, &offset);

        // MAYBE TEMP: push it by names here
        _fl_cmp_byte_code_push_id_access_by_name(&cnt->byte_code, id_chain);
        // Alt:
        // _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);
    }
    // If id is unrecognized just push id chain string
    // (put responsibility on the interpreter to handle import etc.)
    else
        _fl_cmp_byte_code_push_id_access_by_name(&cnt->byte_code, id_chain);

    fl_int_vec_free(&inds);
}

fl_void _fl_cmp_byte_code_push_id_access_by_inds(fl_byte_vec *code, fl_uint offset, const fl_int_vec inds)
{
    fl_byte_vec_push(code, FL_INSTR_GET_ID_BY_INDS);

    // Push scope back offset
    fl_byte_vec_push_mem(code, &offset, sizeof(offset));
    // Push size of indexes
    fl_byte_vec_push_mem(code, &inds.size, sizeof(inds.size));
    // Push actual indexes
    fl_byte_vec_push_mem(code, inds.data, sizeof(inds.data[0]) * inds.size);
}

FL_SEALED_FN fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt)
{
    // Create dummy id in base scope
    fl_id_entry *id;
    fl_id_entry_init(&id, fl_str_dup(name), FL_DATA_TYPE_ANY, fl_value_empty());
    fl_id_entry_vec_push(&cnt->scope->ids, id);

    return id;
}

FL_SEALED_FN fl_void _fl_cmp_paste_byte_code_header(fl_byte_vec *bytes)
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
    fl_byte_vec_push(code, FL_INSTR_GET_ID_BY_NAMES);
    fl_byte_vec_push_str(code, buffer);
}

fl_void fl_cmp_call_to_bytes(
    const fl_ast_node *id_node,
    const fl_operation op,
    fl_cmp_context *cnt)
{
    if (op.type == FL_OPERATION_INSTR)
    {
        fl_byte_vec_push(&cnt->byte_code, op.instr);
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

    // Compile args
    fl_ast_node *arg_node = id_node->next;

    while (arg_node)
    {
        fl_cmp_compile_node(arg_node, cnt);
        arg_node = arg_node->next;
    }

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_EXECUTE);
}

fl_void fl_cmp_block_cnt_to_bytes(
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