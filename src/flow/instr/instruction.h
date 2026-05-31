#pragma once

#include "vecs/value_vec.h"

// REMEMBER TO KEEP IT UP-TO-DATE
#define FL_INSTR_PREDEF_COUNT (FL_INSTR_POW + 1)

FL_FORWARD_STRUCT(fl_context);

/**
 * @brief Represents byte code instructions.
 */
enum _fl_instruction
{
    FL_INSTR_NULL,
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
    FL_INSTR_IMMED_VALUE,
    FL_INSTR_START_BLOCK,
    FL_INSTR_END_BLOCK,
    // FL_INSTR_START_ARGS,
    FL_INSTR_END_ARGS,
};
typedef fl_byte fl_instruction;

FL_HEADER_FN fl_bool fl_instruction_is_callable(fl_instruction instr);

fl_value fl_instruction_call(fl_instruction instr, fl_context *cnt, fl_value_vec args);

FL_HEADER_FN fl_bool fl_instruction_is_callable(fl_instruction instr)
{
    return instr < FL_INSTR_CALL;
}