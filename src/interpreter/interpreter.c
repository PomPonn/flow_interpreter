#include "interpreter.h"

#include "tools/byte_reader.h"
#include "lang/instructions.h"

fl_byte_vec fl_intpr_fn_call_const(
    fl_context *cnt,
    fl_scope *scope,
    fl_fn_variant fn,
    fl_byte_vec args_code)
{
    fl_byte_reader reader = fl_byte_reader_create(args_code);
    while (FL_TRUE)
    {
        fl_byte instr = fl_byte_reader_next(reader, fl_byte);
        switch (instr)
        {
        case FL_INSTR_GET_ID:

            break;

        default:
            break;
        }
    }
}
