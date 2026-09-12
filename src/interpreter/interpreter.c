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
#include "vecs/int_vec.h"
#include "flow/section_mark.h"
#include "macros/basic.h"
#include "flow/id/id_chain.h"

// ------------- MACROS -----------------

#define INTPR_REQUIRED_BC_VERSION _BYTECODE_VERSION

// ---------- DECLARATIONS ----------

// converts bytes in format: <i count><indices...> to fl_int_vec
const fl_int_vec _fl_intpr_indices_from_bytes(fl_byte_reader *reader);

fl_id_entry *_fl_intpr_resolve_id_get_by_inds(fl_byte_reader *reader, fl_scope *scope);

fl_id_entry *_fl_intpr_resolve_id_get_by_names(fl_byte_reader *reader, fl_scope *scope);

fl_void _fl_intpr_parse_header_section(fl_byte_reader *reader);

fl_void _fl_intpr_parse_literals_section(fl_byte_reader *reader, fl_intpr_context *cnt);

inline fl_void _fl_intpr_interpret_code_section(fl_byte_reader *reader, fl_intpr_context *cnt);

fl_void _fl_intpr_interpret_bytes(fl_byte_reader *reader, fl_intpr_context *cnt);

// ---------- DEFINITIONS -----------

fl_void fl_intpr_interpret(fl_byte_vec bytes)
{
    // init blank context
    fl_base_context base_cnt;
    fl_intpr_context cnt;
    fl_base_context_init(&base_cnt);
    fl_intpr_context_init(&cnt, &base_cnt);

    fl_byte_reader reader = fl_byte_reader_create(bytes);

    _fl_intpr_parse_header_section(&reader);
    _fl_intpr_parse_literals_section(&reader, &cnt);

    _fl_intpr_interpret_code_section(&reader, &cnt);

    fl_intpr_context_free(&cnt);
    fl_base_context_free(&base_cnt);
}

// returns last computed fl_value if it was the last operation on stack, otherwise empty fl_value
fl_value fl_intpr_eval(fl_byte_vec bytes, fl_intpr_context *cnt)
{
    fl_byte_reader reader = fl_byte_reader_create(bytes);

    _fl_intpr_interpret_bytes(&reader, cnt);

    fl_operation last_op = fl_op_vec_peek(&cnt->op_stack);
    return last_op.type == FL_OPERATION_VALUE ? last_op.value : FL_VALUE_EMPTY;
}

fl_void _fl_intpr_interpret_bytes(fl_byte_reader *reader, fl_intpr_context *cnt)
{
    fl_bool call_attached = FL_FALSE;

    while (!fl_byte_reader_is_end(reader))
    {
        fl_instruction instr = fl_byte_reader_next(reader, fl_instruction);

        switch (instr)
        {
        case FL_INSTR_START_BLOCK:
            fl_intpr_context_deepen(cnt);
            call_attached = FL_FALSE;
            break;
        case FL_INSTR_END_BLOCK:
        {
            fl_operation op;
            fl_bool complete = FL_TRUE;

            // TO CHANGE
            break;

            if (fl_operation_is_initialized(fl_op_vec_pop(&cnt->op_stack)))
                assert(FL_FALSE);

            // while (!fl_operation_is_callable(op = fl_op_vec_pop(&cnt->op_stack)))
            // {
            //     if (fl_operation_is_instr(op, FL_INSTR_START_BLOCK))
            //     {
            //         cnt->scope = cnt->scope->outer;
            //         complete = FL_TRUE;
            //     }
            // }

            if (!complete)
                assert(FL_FALSE);

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

            fl_value val = fn_id->value;
            assert(val.data_type == FL_DATA_TYPE_FN);

            fl_op_vec_push(&cnt->op_stack, fl_operation_from_fn_vars(val.data));

            call_attached = FL_TRUE;
            break;
        }
        case FL_INSTR_DECL_ID:
        {
            fl_id_entry *new_id;
            fl_char *id_name = fl_str_dup(fl_byte_reader_next_str(reader));
            fl_id_entry_init(&new_id, id_name, FL_DATA_TYPE_ANY, FL_VALUE_EMPTY, FL_FALSE);
            fl_id_entry_vec_push(&cnt->scope->ids, new_id);
            break;
        }
        case FL_INSTR_GET_ID_BY_INDS:
        {
            fl_id_entry *id = _fl_intpr_resolve_id_get_by_inds(reader, cnt->scope);
            assert(id);

            fl_value ref = fl_value_create(FL_DATA_TYPE_REF, id);
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(ref));
            break;
        }
        case FL_INSTR_GET_ID_BY_NAMES:
        {
            fl_id_entry *id = _fl_intpr_resolve_id_get_by_names(reader, cnt->scope);
            fl_value ref = fl_value_create(FL_DATA_TYPE_REF, id);
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(ref));
            break;
        }
        case FL_INSTR_GET_LITERAL:
        {
            const fl_size ind = fl_byte_reader_next(reader, fl_size);
            fl_value lit_val = cnt->base->literals.data[ind];
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(lit_val));
            break;
        }
        case FL_INSTR_VALUE:
        {
            fl_value val = fl_value_from_bytes(reader);
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(val));
            break;
        }
        case FL_INSTR_END_ARGS:
        {
            // consume arguments
            fl_value_vec args;
            fl_value_vec_init(&args);

            fl_operation op;
            while (!fl_operation_is_callable(op = fl_op_vec_pop(&cnt->op_stack)))
                fl_value_vec_put_back(&args, op.value);

            // execute operation
            fl_value res = fl_operation_run(op, args, cnt->base);
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_value(res));

            call_attached = FL_FALSE;
            break;
        }
        default:
            // CALLABLE INSTRUCTION
            fl_op_vec_push(&cnt->op_stack, fl_operation_from_instr(instr));
            break;
        }
    }
}

inline fl_void _fl_intpr_interpret_code_section(fl_byte_reader *reader, fl_intpr_context *cnt)
{
    if (fl_byte_reader_next(reader, fl_section_mark) != FL_SECTION_MARK_CODE)
        assert(FL_FALSE);

    _fl_intpr_interpret_bytes(reader, cnt);
}

fl_void _fl_intpr_parse_literals_section(fl_byte_reader *reader, fl_intpr_context *cnt)
{
    if (fl_byte_reader_next(reader, fl_section_mark) != FL_SECTION_MARK_LITERALS)
        assert(FL_FALSE);

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

// Returned indices are READONLY
const fl_int_vec _fl_intpr_indices_from_bytes(fl_byte_reader *reader)
{
    fl_size count = fl_byte_reader_next(reader, fl_size);
    fl_int *data = fl_byte_reader_next_bytes(reader, sizeof(fl_int) * count);

    return (fl_int_vec){
        .data = data,
        .size = count,
        ._capacity = count,
    };
}

fl_id_entry *_fl_intpr_resolve_id_get_by_inds(fl_byte_reader *reader, fl_scope *scope)
{
    fl_uint offset = fl_byte_reader_next(reader, fl_uint);
    // indices to use to traverse scopes. They are not freed,
    // because they are created unnormally and refer directly to byte code data.
    const fl_int_vec inds = _fl_intpr_indices_from_bytes(reader);

    return fl_scope_traverse(scope, inds, offset);
}

fl_id_entry *_fl_intpr_resolve_id_get_by_names(fl_byte_reader *reader, fl_scope *scope)
{
    const fl_char *id_str = fl_byte_reader_next_str(reader);

    fl_id_chain chain;
    fl_id_chain_init_parse(&chain, id_str);

    return fl_scope_resolve_id(scope, &chain);
}