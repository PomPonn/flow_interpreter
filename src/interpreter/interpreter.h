/**
 * ---------- interpreter.c ----------
 *
 * Declares interpreter logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "lang/context.h"
#include "lang/fn_variant.h"
#include "vecs/byte_vec.h"

// ---------- DECLARATIONS ----------

fl_byte_vec fl_intpr_interpret(fl_byte_vec bytes);