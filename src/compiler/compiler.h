#pragma once

#include "shared/common.h"
#include "lang/context.h"

enum _fl_comp_instruction
{
    // arithmetic
    FL_COMP_INSTR_ADD,
    FL_COMP_INSTR_SUBS,
    FL_COMP_INSTR_MULT,
    FL_COMP_INSTR_DIV,
    FL_COMP_INSTR_POW,
    FL_COMP_INSTR_MOD,
    // ...
    FL_COMP_INSTR_STARTBLOCK,
    FL_COMP_INSTR_ENDBLOCK,
    FL_COMP_INSTR_CALL,
};

fl_void fl_comp_compile(fl_char *source_path, fl_context *context);
