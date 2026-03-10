#pragma once

#include "shared/common.h"
#include "lang/context.h"

enum _fl_cmp_instruction
{
    // arithmetic
    FL_INSTR_ADD,
    FL_INSTR_SUBS,
    FL_INSTR_MULT,
    FL_INSTR_DIV,
    FL_INSTR_POW,
    FL_INSTR_MOD,
    // ...
    FL_INSTR_STARTBLOCK,
    FL_INSTR_ENDBLOCK,
    FL_INSTR_STARTOP,
    FL_INSTR_ENDOP,
    FL_INSTR_CALL,
};

fl_void fl_comp_compile(fl_char *source_path, fl_context *context);
