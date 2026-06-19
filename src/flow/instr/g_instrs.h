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

FL_FORWARD_STRUCT(fl_fn_interface);
FL_FORWARD_STRUCT(fl_const_cstr_vec);
FL_FORWARD_STRUCT(fl_fn_variant_vec);

typedef fl_byte fl_instruction;
typedef fl_byte fl_fl_traits;

// ---------- DECLARATIONS --------------

fl_void fl_g_instrs_init();

fl_void fl_g_instrs_free();

fl_fn_variant_vec *fl_g_instrs_get_variants_of(fl_instruction instr);

const fl_const_cstr_vec *fl_g_instrs_get_escapes();

fl_instruction fl_g_instrs_resolve_alias(const fl_char *alias);