/**
 * ---------- interpreter.h -------------
 *
 * Declares interpreter logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_value);
FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_intpr_context);

// ---------- DECLARATIONS ----------

fl_void fl_intpr_interpret(fl_byte_vec bytes);

// *unused*
fl_value fl_intpr_eval(fl_byte_vec bytes, fl_intpr_context *cnt);