#include "interpreter.h"

#include "tools/byte_reader.h"
#include "flow/instr/instruction.h"
#include "flow/context.h"

fl_void fl_intpr_interpret(fl_byte_vec bytes, fl_context *cnt)
{
    fl_scope current_scope = cnt->main_scope;
    fl_byte_reader reader = fl_byte_reader_create(bytes);

    while (FL_TRUE)
    {
        fl_instruction instr = fl_byte_reader_next(&reader, fl_instruction);

        switch (instr)
        {
        case FL_INSTR_START_BLOCK:

            break;

        default:
            // fl_instruction_call(instr, cnt, );
            break;
        }
    }
}