/**
 * ---------- compiler.h --------------
 *
 * Declares compiler logic.
 *
 * ------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES -----------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS -----------

FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_ast_node);

// ---------- DECLARATIONS -----------

fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *node);