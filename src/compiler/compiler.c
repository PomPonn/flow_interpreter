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
#include "compiler/shared.h"
#include "tools/time.h"

// ---------- DECLARATIONS ----------

fl_void _fl_cmp_compile_block(const fl_ast_node *node, fl_cmp_context *cnt);
fl_void _fl_cmp_compile_op_call(const fl_ast_node *op_node, fl_cmp_context *cnt);
fl_void _fl_cmp_compile_plain_id_chain(const fl_ast_node *node, fl_cmp_context *cnt);
fl_void _fl_cmp_stamp_id_access(fl_cmp_context *cnt, fl_ast_node *id_node);
fl_void _fl_cmp_compile_literal(const fl_ast_node *node, fl_cmp_context *cnt);
inline fl_void _fl_cmp_compile_args(const fl_ast_node *arg_node, fl_cmp_context *cnt);
inline fl_void _fl_cmp_byte_code_push_id_access_by_inds(fl_byte_vec *code, fl_uint offset, fl_int_vec inds);
inline fl_void _fl_cmp_byte_code_push_id_access_by_names(fl_byte_vec *code, const fl_id_chain *chain);
inline fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt);
inline fl_void _fl_cmp_paste_byte_code_header(fl_byte_vec *bytes);
inline fl_size _fl_cmp_make_literal(fl_value lit_value, fl_cmp_context *cnt);

// ---------- DEFINITIONS ----------

fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *tree)
{
    fl_base_context base;
    fl_base_context_init(&base);

    // init blank context
    fl_cmp_context cnt;
    fl_cmp_context_init(&cnt, &base);

    // compile nodes
    fl_cmp_compile_node(tree, &cnt);

    fl_byte_vec compiled;
    fl_byte_vec_init(&compiled);

    // add byte code version info
    _fl_cmp_paste_byte_code_header(&compiled);

    if (!fl_value_vec_is_empty(&cnt.base->literals))
    {
        // add literals table
        const fl_size n = cnt.base->literals.size;

        fl_byte_vec_push(&compiled, FL_SECTION_MARK_LITERALS);
        fl_byte_vec_push_t(&compiled, n, fl_size);
        for (fl_size i = 0; i < n; i++)
            fl_value_to_bytes(cnt.base->literals.data[i], &compiled);
    }

    // merge both byte vecs
    fl_byte_vec_push(&compiled, FL_SECTION_MARK_CODE);
    fl_byte_vec_merge(&compiled, &cnt.byte_code);

    // free remains
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
        // fn call and operator generalized to operation struct
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

fl_void _fl_cmp_compile_block(const fl_ast_node *node, fl_cmp_context *cnt)
{
    fl_cmp_context inner_cnt;
    fl_cmp_context_from(&inner_cnt, (fl_local_context *)cnt);
    fl_local_context_deepen((fl_local_context *)&inner_cnt);

    node = node->child;
    while (node)
    {
        fl_cmp_compile_node(node, &inner_cnt);
        node = node->next;
    }

    // convert whole context to bytes
    fl_cmp_block_cnt_to_bytes(&cnt->byte_code, &inner_cnt);

    fl_cmp_context_free(&inner_cnt);
}

fl_void fl_cmp_call_to_bytes(fl_cmp_context *cnt, fl_ast_node *id_node, fl_operation op)
{
    if (op.type == FL_OPERATION_INSTR)
        fl_byte_vec_push(&cnt->byte_code, op.instr);
    else if (fl_bits_is_any(op.type, FL_OPERATION_FN | FL_OPERATION_UNKNOWN))
    {
        // start CALL instruction
        fl_byte_vec_push(&cnt->byte_code, FL_INSTR_CALL);
        // push information how to access id
        _fl_cmp_stamp_id_access(cnt, id_node);
    }
    else
        assert(FL_FALSE);

    // compile args
    _fl_cmp_compile_args(id_node->next, cnt);
}

fl_void _fl_cmp_compile_op_call(const fl_ast_node *op_node, fl_cmp_context *cnt)
{
    fl_ast_node *id_node = op_node->child;
    fl_operation op = fl_operation_from_id_node(id_node, cnt->scope);

    if (op.type != FL_OPERATION_UNKNOWN)
    {
        if (!fl_operation_is_callable(op))
            assert(FL_FALSE);

        // try to inline at first
        if (fl_inl_try_op_inline(id_node, op, cnt))
            return;

        // throw an error if compile-time-only function wasn't inlined
        if (fl_operation_is_compile_time(op))
            assert(FL_FALSE);
    }

    // compile it normally
    fl_cmp_call_to_bytes(cnt, id_node, op);
}

fl_void _fl_cmp_stamp_id_access(fl_cmp_context *cnt, fl_ast_node *id_node)
{
    fl_id_chain *id_chain = id_node->data;

    fl_uint offset;
    fl_int_vec inds = fl_scope_find_id_path(cnt->scope, id_chain, &offset);

    if (fl_int_vec_is_empty(&inds))
        _fl_cmp_byte_code_push_id_access_by_names(&cnt->byte_code, id_chain);
    else
        _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);

    fl_int_vec_free(&inds);
}

inline fl_size _fl_cmp_make_literal(fl_value lit_value, fl_cmp_context *cnt)
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

fl_void _fl_cmp_compile_plain_id_chain(const fl_ast_node *node, fl_cmp_context *cnt)
{
    fl_id_chain *id_chain = (fl_id_chain *)node->data;

    // try to find this id
    fl_uint offset;
    fl_int_vec inds = fl_scope_find_id_path(cnt->scope, id_chain, &offset);
    fl_id_entry *id = fl_scope_traverse(cnt->scope, inds, offset);

    // push opted info how to to access this id if it's found
    // (may be to change)
    if (id)
        _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);
    // create new id if accessor isn't chained
    else if (id_chain->size == 1)
    {
        const fl_char *name = id_chain->data[0];
        // handle new id creation
        id = _fl_cmp_handle_new_id(name, cnt);

        // find new ids path
        fl_int_vec_free(&inds);
        inds = fl_scope_find_id_path(cnt->scope, id_chain, &offset);

        _fl_cmp_byte_code_push_id_access_by_inds(&cnt->byte_code, offset, inds);
    }
    // if id is unrecognized just push id chain string
    // (put responsibility on the interpreter to handle import etc.)
    else
        _fl_cmp_byte_code_push_id_access_by_names(&cnt->byte_code, id_chain);

    fl_int_vec_free(&inds);
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

inline fl_void _fl_cmp_compile_args(const fl_ast_node *arg_node, fl_cmp_context *cnt)
{
    while (arg_node)
    {
        fl_cmp_compile_node(arg_node, cnt);
        arg_node = arg_node->next;
    }

    fl_byte_vec_push(&cnt->byte_code, FL_INSTR_EXECUTE);
}

inline fl_id_entry *_fl_cmp_handle_new_id(const fl_char *name, fl_cmp_context *cnt)
{
    // create dummy id in base scope
    fl_id_entry *id;
    fl_id_entry_init(&id, fl_str_dup(name), FL_DATA_TYPE_ANY, fl_value_empty());
    fl_id_entry_vec_push(&cnt->scope->ids, id);

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

inline fl_void _fl_cmp_byte_code_push_id_access_by_names(fl_byte_vec *code, const fl_id_chain *chain)
{
    fl_char buffer[FL_ID_CHAIN_MAX_SIZE];
    fl_id_chain_to_string(chain, buffer, FL_ID_CHAIN_MAX_SIZE);
    fl_byte_vec_push(code, FL_INSTR_GET_ID_BY_NAMES);
    fl_byte_vec_push_str(code, buffer);
}