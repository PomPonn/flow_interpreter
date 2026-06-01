/**
 * ---------- g_instrs.h --------------------
 *
 * Declares global instructions logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_fn_interface);
FL_FORWARD_STRUCT(fl_const_cstr_vec);
FL_FORWARD_STRUCT(fl_instr_alias);
typedef fl_byte fl_instruction;
typedef fl_byte fl_fl_traits;

// ---------- DECLARATIONS --------------

fl_void fl_g_instrs_init();

fl_void fl_g_instrs_free();

const fl_fn_interface *fl_g_instrs_get_interface(fl_instruction instr);

const fl_const_cstr_vec *fl_g_instrs_aliases_to_escapes();

const fl_instr_alias *fl_g_instrs_find_alias(const fl_char *alias);