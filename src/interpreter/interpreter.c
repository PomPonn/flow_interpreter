#include "interpreter.h"

#include "tools/byte_reader.h"
#include "flow/instr/instruction.h"
#include "flow/context.h"
#include "vecs/int_vec.h"
#include "vecs/op_vec.h"

// converts bytes in format: <len><indices...> to fl_int_vec
const fl_int_vec _fl_intpr_indices_from_bytes(fl_byte_reader *reader);

fl_id_entry *_fl_intpr_resolve_id_get(fl_byte_reader *reader, fl_scope *scope);

fl_void fl_intpr_interpret(fl_byte_vec bytes)
{
    // init blank context
    fl_context cnt;
    fl_context_init(&cnt);
    fl_global_put_static_predefs(&cnt);

    fl_scope *current_scope = &cnt.main_scope;
    fl_byte_reader reader = fl_byte_reader_create(bytes);

    fl_op_vec op_stack;
    fl_op_vec_init(&op_stack);

    while (FL_TRUE)
    {
        fl_instruction instr = fl_byte_reader_next(&reader, fl_instruction);

        switch (instr)
        {
        case FL_INSTR_START_BLOCK:
        {
            fl_scope *inner = fl_mem_alloc_t(fl_scope);
            fl_scope_init(inner, current_scope);
            current_scope = inner;
            break;
        }
        case FL_INSTR_END_BLOCK:
        {
            fl_operation op = fl_op_vec_pop(&op_stack);
            if (!fl_operation_is_instr(op, FL_INSTR_START_BLOCK))
                assert(FL_FALSE);

            current_scope = current_scope->outer;
            break;
        }
        case FL_INSTR_CALL:
        {
            fl_id_entry *fn_id = _fl_intpr_resolve_id_get(&reader, current_scope);
            fl_value val = fn_id->value;
            assert(val.data_type == FL_DATA_TYPE_FN);

            fl_op_vec_push(&op_stack, fl_operation_from_fn_vars(val.data));
            break;
        }
        case FL_INSTR_NEW_ID:
        {
            fl_id_entry *new_id;
            fl_char *id_name = fl_str_dup(fl_byte_reader_next_str(&reader));
            fl_id_entry_init(&new_id, id_name, FL_DATA_TYPE_ANY, FL_VALUE_EMPTY, FL_FALSE);
            fl_id_entry_vec_push(&current_scope->ids, new_id);
            break;
        }
        case FL_INSTR_GET_ID:
        {
            fl_id_entry *id = _fl_intpr_resolve_id_get(&reader, current_scope);
            fl_op_vec_push(&op_stack, fl_operation_from_value(&id->value));
            break;
        }
        case FL_INSTR_GET_LITERAL:
        {

            break;
        }
        case FL_INSTR_IMMED_VALUE:
        {

            break;
        }
        default:
            // CALLABLE INSTRUCTION

            fl_op_vec_push(&op_stack, fl_operation_from_instr(instr));
            break;
        }
    }

    fl_context_free(&cnt);
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

fl_id_entry *_fl_intpr_resolve_id_get(fl_byte_reader *reader, fl_scope *scope)
{
    fl_uint offset = fl_byte_reader_next(reader, fl_uint);
    // indices to use to traverse scopes. They are not freed,
    // because they are created unnormally and refer directly to byte code data.
    const fl_int_vec inds = _fl_intpr_indices_from_bytes(reader);

    return fl_scope_traverse(scope, inds, offset);
}