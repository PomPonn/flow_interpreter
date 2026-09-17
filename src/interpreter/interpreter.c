/**
 * ---------- interpreter.c ---------------
 *
 * Implements interpreter logic.
 *
 * ----------------------------------------
 */

#include "interpreter.h"

// ---------- DEPENDENCIES --------------

#include "tools/byte_reader.h"
#include "flow/instr/instruction.h"
#include "context/intpr_context.h"
#include "context/cmp_context.h"
#include "vecs/int_vec.h"
#include "flow/section_mark.h"
#include "macros/basic.h"
#include "flow/id/id_chain.h"
#include "flow/byte_segm.h"

#ifndef NDEBUG

#include "tools/printer.h"

#endif

// ------------- MACROS -----------------

#define INTPR_REQUIRED_BC_VERSION _BYTECODE_VERSION

// ---------- DECLARATIONS ----------

// converts bytes in format: <i count><indices...> to fl_int_vec
fl_int_vec _fl_intpr_indices_from_bytes(fl_byte_reader *reader);
fl_id_entry *_fl_intpr_resolve_id_get_by_names(fl_byte_reader *reader, fl_scope *scope);
fl_void _fl_intpr_parse_header_section(fl_byte_reader *reader);
fl_void _fl_intpr_parse_literals_section(fl_byte_reader *reader, fl_intpr_context *cnt);
fl_void _fl_intpr_interpret_bytes(fl_byte_reader *reader, fl_intpr_context *cnt);
FL_SEALED_FN fl_id_entry *_fl_intpr_resolve_id_get_by_inds(fl_byte_reader *reader, fl_scope *scope);
FL_SEALED_FN fl_void _fl_intpr_interpret_code_section(fl_byte_reader *reader, fl_intpr_context *cnt);
FL_SEALED_FN fl_void _fl_intpr_push_ref(fl_intpr_context *cnt, fl_id_entry *id);

// frees passed args when function execution is completed
fl_value _fl_intpr_exec_fn(const fl_fn_variant *var, fl_value_vec *args, const fl_local_context *local_cnt);

// ---------- DEFINITIONS -----------

fl_void fl_intpr_interpret(fl_byte_vec code)
{
    // init blank context
    fl_base_context base_cnt;
    fl_intpr_context cnt;
    fl_base_context_init(&base_cnt);
    fl_intpr_context_init(&cnt, &base_cnt);

    fl_byte_reader reader = fl_byte_reader_create(code);

    _fl_intpr_parse_header_section(&reader);
    _fl_intpr_parse_literals_section(&reader, &cnt);

    _fl_intpr_interpret_code_section(&reader, &cnt);

    fl_intpr_context_free(&cnt);
    fl_base_context_free(&base_cnt);
}

fl_value fl_intpr_eval(fl_byte_segm code, fl_local_context *cnt)
{
    fl_intpr_context intpr_cnt;
    fl_intpr_context_from(&intpr_cnt, cnt);

    fl_byte_reader reader = fl_byte_reader_create_segm(code);

    _fl_intpr_interpret_bytes(&reader, &intpr_cnt);

    fl_operation last_op = fl_op_vec_pop(&intpr_cnt.op_stack);

    fl_intpr_context_free(&intpr_cnt);
    return last_op.type == FL_OPERATION_VALUE ? last_op.value : fl_value_empty();
}

fl_void _fl_intpr_interpret_bytes(fl_byte_reader *reader, fl_intpr_context *cnt)
{
    // should it be bond to fl_scope?
    fl_bool call_attached = FL_FALSE;

    while (!fl_byte_reader_is_end(reader))
    {
        fl_instruction instr = fl_byte_reader_next(reader, fl_instruction);

        switch (instr)
        {
        case FL_INSTR_START_BLOCK:
            if (call_attached)
            {
                fl_data_desc desc = fl_data_desc_create_traited(FL_DATA_TYPE_CODE_SEGM, FL_DATA_TRAIT_CONST);

                fl_byte_segm *segm = fl_mem_alloc_t(fl_byte_segm);
                // extract block segment (skips it too)
                *segm = fl_byte_reader_extract_block_segm(reader);
                fl_value cptr = fl_value_create_desc(desc, segm);
                fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(cptr));

                call_attached = FL_FALSE;
            }
            else
            {
                fl_local_context_deepen((fl_local_context *)cnt);
                // skip block size
                fl_byte_reader_skip(reader, fl_size);
            }

            break;
        case FL_INSTR_END_BLOCK:
        {
            fl_local_context_shallow((fl_local_context *)cnt);

            // if this block is nested call attach won't work
            // TODO: make it per scope
            call_attached = FL_FALSE;
            break;
        }
        case FL_INSTR_CALL:
        {
            fl_id_entry *fn_id = FL_NULL;

            switch (fl_byte_reader_next(reader, fl_instruction))
            {
            case FL_INSTR_GET_ID_BY_INDS:
                fn_id = _fl_intpr_resolve_id_get_by_inds(reader, cnt->scope);
                break;
            case FL_INSTR_GET_ID_BY_NAMES:
                fn_id = _fl_intpr_resolve_id_get_by_names(reader, cnt->scope);
                break;
            default:
                assert(FL_FALSE);
                break;
            }
            assert(fn_id);

            fl_value val = fn_id->value;
            assert(val.desc.type == FL_DATA_TYPE_FN);

            fl_op_vec_push(&cnt->op_stack, fl_operation_from_fn_vars(val.data));

            call_attached = FL_TRUE;
            break;
        }
        case FL_INSTR_DECL_ID:
        {
            fl_id_entry *new_id;
            fl_char *id_name = fl_str_dup(fl_byte_reader_next_str(reader));
            fl_id_entry_init(&new_id, id_name, FL_DATA_TYPE_ANY, fl_value_empty());
            fl_id_entry_vec_push(&cnt->scope->ids, new_id);
            break;
        }
        case FL_INSTR_GET_ID_BY_INDS:
        {
            fl_id_entry *id = _fl_intpr_resolve_id_get_by_inds(reader, cnt->scope);
            assert(id);
            _fl_intpr_push_ref(cnt, id);
            break;
        }
        case FL_INSTR_GET_ID_BY_NAMES:
        {
            fl_id_entry *id = _fl_intpr_resolve_id_get_by_names(reader, cnt->scope);
            assert(id);
            _fl_intpr_push_ref(cnt, id);
            break;
        }
        case FL_INSTR_GET_LITERAL:
        {
            const fl_size ind = fl_byte_reader_next(reader, fl_size);
            fl_value lit_val = cnt->base->literals.data[ind];
            // make this a literal view
            fl_bits_set(lit_val.desc.traits, FL_DATA_TRAIT_VIEW);
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(lit_val));
            break;
        }
        case FL_INSTR_VALUE:
        {
            fl_value val = fl_value_from_bytes(reader);
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(val));
            break;
        }
        case FL_INSTR_EXECUTE:
        {
            // consume arguments
            fl_value_vec args;
            fl_value_vec_init(&args);

            fl_operation op = fl_operation_uninitialized();
            while (!fl_operation_is_callable(op = fl_op_vec_pop(&cnt->op_stack)))
            {
                assert(fl_operation_is_initialized(op));
                fl_value_vec_put_back(&args, op.value);
            }

            // execute operation (last popped operation is function op)
            fl_fn_variant *var = fl_operation_prep_run(op, &args, cnt->base);
            fl_value res = _fl_intpr_exec_fn(var, &args, (fl_local_context *)cnt);
            // no need to free the args, it's handled by _fl_intpr_exec_fn function

            // push value even if it's empty? (as in assign case)
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(res));

            call_attached = FL_FALSE;
            break;
        }
        default:
            assert(instr > FL_INSTR_NONE && instr <= FL_INSTR_POW);

            // CALLABLE INSTRUCTION
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_instr(instr));

            call_attached = FL_TRUE;
            break;
        }
    }
}

fl_value _fl_intpr_exec_fn(const fl_fn_variant *fn_var, fl_value_vec *args, const fl_local_context *local_cnt)
{
    if (fn_var->c_defined)
    {
        fl_value res = fn_var->fn_ptr(*args);
        fl_value_vec_free(args);
        return res;
    }

    // prepare context
    fl_local_context cnt;
    fl_local_context_from(&cnt, local_cnt);
    fl_local_context_deepen(&cnt);

    fl_data_desc_vec descs = fn_var->intf.param_descs;
    fl_cstr_vec labels = fn_var->intf.param_labels;

    assert(descs.size == labels.size && labels.size == args->size);

    // add args to context as ids
    for (fl_size i = 0; i < args->size; i++)
    {
        // TODO: extend this
        fl_id_entry *id;
        fl_id_entry_init(&id, fl_str_dup(labels.data[i]), descs.data[i].type, args->data[i]);
        fl_id_entry_vec_push(&cnt.scope->ids, id);
    }

    // inner block start is already handled
    assert(*fn_var->body_ptr->data == FL_INSTR_START_BLOCK);

    // skip block start and block size
    const fl_ushort diff = sizeof(fl_size) + 1;
    fl_byte_segm body_content = fl_byte_segm_create(
        fn_var->body_ptr->data + diff,
        fn_var->body_ptr->len - diff);

    fl_value res = fl_intpr_eval(body_content, &cnt);

    // free array only
    // each individual arg will is freed through the context
    fl_value_vec_free_shallow(args);
    // Don't free local scope, because it's already invalid and handled by eval function

    return res;
}

FL_SEALED_FN fl_void _fl_intpr_interpret_code_section(fl_byte_reader *reader, fl_intpr_context *cnt)
{
    if (fl_byte_reader_next(reader, fl_section_mark) != FL_SECTION_MARK_CODE)
        assert(FL_FALSE);

    _fl_intpr_interpret_bytes(reader, cnt);
}

fl_void _fl_intpr_parse_literals_section(fl_byte_reader *reader, fl_intpr_context *cnt)
{
    if (fl_byte_reader_next(reader, fl_section_mark) != FL_SECTION_MARK_LITERALS)
        return;

    const fl_size lits_count = fl_byte_reader_next(reader, fl_size);

    for (fl_size i = 0; i < lits_count; i++)
    {
        fl_value val = fl_value_from_bytes(reader);
        fl_value_vec_push(&cnt->base->literals, val);
    }
}

fl_void _fl_intpr_parse_header_section(fl_byte_reader *reader)
{
    if (fl_byte_reader_next(reader, fl_section_mark) != FL_SECTION_MARK_HEADER)
        assert(FL_FALSE);

    const fl_char *ver = fl_byte_reader_next_str(reader);

    if (!fl_str_equal(ver, INTPR_REQUIRED_BC_VERSION))
        // invalid bc version
        assert(FL_FALSE);

    fl_long creation_timestamp = fl_byte_reader_next(reader, fl_long);

    // do sth with this info...
}

fl_int_vec _fl_intpr_indices_from_bytes(fl_byte_reader *reader)
{
    fl_size count = fl_byte_reader_next(reader, fl_size);
    fl_size mem_size = sizeof(fl_int) * count;

    fl_int *data = fl_mem_alloc_arr_t(fl_int, count);
    const fl_unknown *mem = fl_byte_reader_next_bytes(reader, mem_size);
    fl_mem_copy(mem, data, mem_size);

    return (fl_int_vec){
        .data = data,
        .size = count,
        ._capacity = count,
    };
}

FL_SEALED_FN fl_id_entry *_fl_intpr_resolve_id_get_by_inds(fl_byte_reader *reader, fl_scope *scope)
{
    fl_uint offset = fl_byte_reader_next(reader, fl_uint);
    fl_int_vec inds = _fl_intpr_indices_from_bytes(reader);

    fl_id_entry *id = fl_scope_traverse(scope, inds, offset);

    fl_int_vec_free(&inds);
    return id;
}

fl_id_entry *_fl_intpr_resolve_id_get_by_names(fl_byte_reader *reader, fl_scope *scope)
{
    const fl_char *id_str = fl_byte_reader_next_str(reader);

    fl_id_chain chain;
    fl_id_chain_init_parse(&chain, id_str);

    return fl_scope_resolve_id(scope, &chain);
}

FL_SEALED_FN fl_void _fl_intpr_push_ref(fl_intpr_context *cnt, fl_id_entry *id)
{
    fl_value ref = fl_value_create_ref(id);
    fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(ref));
}
