#pragma once

// REMEMBER TO KEEP IT UP-TO-DATE
#define FL_PREDEF_INSTRS_COUNT (FL_INSTR_POW + 1)

/**
 * @brief Represents byte code instructions.
 */
enum _fl_instruction
{
    // TODO: comment this when finished

    // callable instructions
    // add new ones before FL_INSTR_POW to avoid dependancy problems
    FL_INSTR_IMPORT,
    FL_INSTR_ASSIGN,
    FL_INSTR_ADD,
    FL_INSTR_SUB,
    FL_INSTR_DIV,
    FL_INSTR_MUL,
    FL_INSTR_MOD,
    FL_INSTR_POW,
    // flow instructions
    FL_INSTR_CALL,
    FL_INSTR_GET_ID,
    FL_INSTR_NEW_ID,
    FL_INSTR_GET_LITERAL,
    FL_INSTR_IMMEDIATE_VALUE,
    FL_INSTR_START_BLOCK,
    FL_INSTR_END_BLOCK,
    FL_INSTR_START_ARGS,
    FL_INSTR_END_ARGS,
};
typedef fl_byte fl_instruction;