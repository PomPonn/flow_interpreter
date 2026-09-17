/**
 * ---------- g_instrs.h ------------------
 *
 * Declares global instructions logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_fn_intf);
FL_FORWARD_STRUCT(fl_const_cstr_vec);
FL_FORWARD_STRUCT(fl_fn_variant_vec);
FL_FORWARD_STRUCT(fl_instr_info_vec);

typedef fl_byte fl_instruction;

// ---------- DECLARATIONS --------------

/**
 * @brief Initialize global instructions.
 */
fl_void fl_g_instrs_init();

/**
 * @brief Free global instructions.
 */
fl_void fl_g_instrs_free();

/**
 * @brief Retrieve global instruction's variants.
 * @param instr Instruction to retrive info about.
 * @return Instruction's variants.
 */
const fl_fn_variant_vec *fl_g_instrs_get_variants_of(fl_instruction instr);

/**
 * @brief Get token values to escape during tokenization.
 * @return Escapes string vector.
 */
const fl_const_cstr_vec *fl_g_instrs_get_escapes();

/**
 * @brief Get global instructions information vector.
 * @return Instructions info vector.
 */
const fl_instr_info_vec *fl_g_instrs_get_info_vec();

/**
 * @brief Resolve instruction based on it's alias.
 * @param alias Alias to look for.
 * @return Found instruction (if any).
 */
fl_instruction fl_g_instrs_resolve_alias(const fl_char *alias);