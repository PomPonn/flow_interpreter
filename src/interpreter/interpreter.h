/**
 * ---------- interpreter.c ----------
 *
 * Declares interpreter logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "vecs/byte_vec.h"

// ---------- DECLARATIONS ----------

FL_FORWARD_STRUCT(fl_context);

fl_void fl_intpr_interpret(fl_byte_vec bytes, fl_context *cnt);