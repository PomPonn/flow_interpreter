/**
 * ---------- globals.h --------------------
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

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_predef);
FL_FORWARD_STRUCT(fl_context);

// ---------- DECLARATIONS --------------

fl_void fl_globals_init();

fl_void fl_globals_free();

const fl_fn_interface *fl_globals_get_static_predef_interface(const fl_predef *predef);

// replaces context predefs with static ones
fl_void fl_globals_put_static_predefs(fl_context *cnt);

fl_const_cstr_vec fl_globals_get_static_predefs_escapes();