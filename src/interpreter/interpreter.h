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
FL_FORWARD_STRUCT(fl_byte_segm);
FL_FORWARD_STRUCT(fl_local_context);

// ---------- DECLARATIONS ----------

fl_void fl_intpr_interpret(fl_byte_vec code);

// returns last computed fl_value if it was the last operation on stack, otherwise empty fl_value
fl_value fl_intpr_eval(fl_byte_segm code, const fl_local_context *cnt);