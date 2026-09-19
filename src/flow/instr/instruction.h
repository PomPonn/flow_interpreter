/**
 * ---------- instruction.h ---------------------
 *
 * Declares Flow instruction logic.
 *
 * ----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/instr/g_instrs.h"

// ------------- MACROS -----------------

// REMEMBER TO KEEP IT UP TO DATE
#define FL_CLL_INSTR_COUNT FL_INSTR_POW

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_base_context);
FL_FORWARD_STRUCT(fl_value_vec);
FL_FORWARD_STRUCT(fl_value);

enum _fl_instruction
{
    // Not an instruction.
    FL_INSTR_NONE,

    /**
     * Callable instructions:
     * -- Add new ones before FL_INSTR_POW to avoid dependancy problems. --
     */

    FL_INSTR_IMPORT,
    FL_INSTR_ASSIGN,
    FL_INSTR_LAMBDA,
    FL_INSTR_ADD,
    FL_INSTR_SUB,
    FL_INSTR_DIV,
    FL_INSTR_MUL,
    FL_INSTR_MOD,
    FL_INSTR_POW,

    /**
     * Byte code instructions:
     */

    FL_INSTR_CALL,
    FL_INSTR_GET_ID,
    FL_INSTR_DECL_ID,
    FL_INSTR_GET_LITERAL,
    FL_INSTR_VALUE,
    FL_INSTR_START_BLOCK,
    FL_INSTR_END_BLOCK,
    FL_INSTR_EXECUTE,
};

/**
 * @brief Flow instructions.
 */
typedef fl_byte fl_instruction;

// ---------- DECLARATIONS ----------

/**
 * @brief Check if instruction is callable.
 * @param instr Instruction to check.
 * @return Boolean value whether instruction is callable.
 */
FL_SEALED_FN fl_bool fl_instruction_is_callable(fl_instruction instr);

/**
 * @brief Get instruction by alias (using global instructions).
 * @param alias Alias to look for.
 * @return Found (or not found) instruction.
 */
FL_SEALED_FN fl_instruction fl_instruction_from_alias(const fl_char *alias);

/**
 * @brief Retrieve instruction callable variants.
 * @param instr Instruction to get info about.
 * @return Instruction variants.
 */
FL_SEALED_FN const fl_fn_variant_vec *fl_instruction_get_variants(fl_instruction instr);

// ---------- DEFINITIONS -----------

FL_SEALED_FN fl_bool fl_instruction_is_callable(fl_instruction instr)
{
    return instr <= FL_INSTR_POW;
}

FL_SEALED_FN const fl_fn_variant_vec *fl_instruction_get_variants(fl_instruction instr)
{
    return fl_instruction_is_callable(instr) ? fl_g_instrs_get_variants_of(instr) : FL_NULL;
}

FL_SEALED_FN fl_instruction fl_instruction_from_alias(const fl_char *alias)
{
    return fl_g_instrs_resolve_alias(alias);
}