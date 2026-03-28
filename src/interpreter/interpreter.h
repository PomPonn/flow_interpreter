/**
 * ---- interpreter.c ----
 *
 * Declares interpreter logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "lang/context.h"
#include "lang/fn_variant.h"
#include "vecs/byte_vec.h"

// ---- DECLARATIONS ----

fl_byte_vec fl_intpr_fn_call_const(
    fl_context *cnt,
    fl_scope *scope,
    fl_fn_variant fn,
    fl_byte_vec args_code);