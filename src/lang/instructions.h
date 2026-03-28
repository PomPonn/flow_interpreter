#pragma once

/**
 * @brief Represents byte code instructions.
 */
enum _fl_cmp_instruction
{
    // TODO: comment this when finished

    // arithmetic
    FL_INSTR_ADD,
    FL_INSTR_SUBS,
    FL_INSTR_MULT,
    FL_INSTR_DIV,
    FL_INSTR_POW,
    FL_INSTR_MOD,
    // ...
    FL_INSTR_CALL,
    FL_INSTR_GET_ID,
    FL_INSTR_GET_LITERAL,
    FL_INSTR_NEW,
    // use questioned
    FL_INSTR_STARTBLOCK,
    FL_INSTR_ENDBLOCK,
    FL_INSTR_STARTOP,
    FL_INSTR_ENDOP,
};