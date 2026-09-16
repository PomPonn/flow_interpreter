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
    FL_INSTR_NONE,
    // TODO: comment this when finished

    // callable instructions
    FL_INSTR_IMPORT,
    FL_INSTR_ASSIGN,
    FL_INSTR_LAMBDA,
    FL_INSTR_ADD,
    FL_INSTR_SUB,
    FL_INSTR_DIV,
    FL_INSTR_MUL,
    FL_INSTR_MOD,
    // add new ones before FL_INSTR_POW to avoid dependancy problems
    FL_INSTR_POW,
    // flow instructions
    FL_INSTR_CALL,
    FL_INSTR_GET_ID_BY_INDS,
    FL_INSTR_GET_ID_BY_NAMES,
    FL_INSTR_DECL_ID,
    FL_INSTR_GET_LITERAL,
    FL_INSTR_VALUE,
    FL_INSTR_START_BLOCK,
    FL_INSTR_END_BLOCK,
    FL_INSTR_EXECUTE,
};
typedef fl_byte fl_instruction;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_bool fl_instruction_is_callable(fl_instruction instr);
FL_HEADER_FN fl_instruction fl_instruction_from_alias(const fl_char *alias);
FL_HEADER_FN fl_fn_variant_vec *fl_instruction_get_variants(fl_instruction instr);

// ---------- DEFINITIONS -----------

FL_HEADER_FN fl_bool fl_instruction_is_callable(fl_instruction instr)
{
    return instr < FL_INSTR_CALL;
}

FL_HEADER_FN fl_fn_variant_vec *fl_instruction_get_variants(fl_instruction instr)
{
    return fl_instruction_is_callable(instr) ? fl_g_instrs_get_variants_of(instr) : FL_NULL;
}

FL_HEADER_FN fl_instruction fl_instruction_from_alias(const fl_char *alias)
{
    return fl_g_instrs_resolve_alias(alias);
}