/**
 * ---------- global.h --------------------
 *
 * Declares global modules logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "flow/fn/fn_interface.h"
#include "vecs/const_cstr_vec.h"
#include "flow/instr/instruction.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_predef);
FL_FORWARD_STRUCT(fl_context);

typedef fl_byte fl_instruction;

// ---------- DECLARATIONS --------------

fl_void fl_global_init();

fl_void fl_global_free();

const fl_fn_interface *fl_global_get_static_predef_interface(const fl_predef *predef);

const fl_fn_interface *fl_global_get_instr_interface(fl_instruction instr);

// replaces context predefs with static ones
fl_void fl_global_put_static_predefs(fl_context *cnt);

fl_const_cstr_vec fl_global_get_static_predefs_escapes();