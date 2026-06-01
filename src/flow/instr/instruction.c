#include "instruction.h"

// ---------- DEPENDENCIES ----------

#include "flow/context.h"
#include "flow/instr/internal/_all.h"

fl_value fl_instruction_call(fl_instruction instr, fl_context *cnt, fl_value_vec args)
{
    switch (instr)
    {
    case FL_INSTR_ADD:
        return fl_instr_add(args.data[0], args.data[1]);
    case FL_INSTR_ASSIGN:
        return fl_instr_assign(args.data[0], &args.data[1]);
    case FL_INSTR_IMPORT:
        return fl_instr_import(cnt, args.data[0].data);
    default:
        assert(FL_FALSE);
        return FL_VALUE_EMPTY;
    }
}