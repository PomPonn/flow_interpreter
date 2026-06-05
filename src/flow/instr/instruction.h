/**
 * ---------- instruction.h -------------------------
 *
 * Declares language (Built-in) instruction logic.
 *
 * --------------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/instr/g_instrs.h"

// ------------- MACROS -----------------

// REMEMBER TO KEEP IT UP-TO-DATE
#define FL_CLL_INSTR_COUNT FL_INSTR_POW

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_base_context);
FL_FORWARD_STRUCT(fl_value_vec);
FL_FORWARD_STRUCT(fl_value);

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
    FL_INSTR_IMMED_VALUE,
    FL_INSTR_START_BLOCK,
    FL_INSTR_END_BLOCK,
    // FL_INSTR_START_ARGS,
    FL_INSTR_END_ARGS,
};
typedef fl_byte fl_instruction;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_bool fl_instruction_is_callable(fl_instruction instr);
FL_HEADER_FN const fl_fn_interface *fl_instruction_get_interface(fl_instruction instr);

fl_value fl_instruction_call(fl_instruction instr, fl_base_context *cnt, fl_value_vec args);

// ---------- DEFINITIONS -----------

FL_HEADER_FN fl_bool fl_instruction_is_callable(fl_instruction instr)
{
    return instr < FL_INSTR_CALL;
}

FL_HEADER_FN const fl_fn_interface *fl_instruction_get_interface(fl_instruction instr)
{
    return fl_g_instrs_get_interface(instr);
}