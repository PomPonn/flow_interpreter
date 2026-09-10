/**
 * ---------- inliner.h --------------------
 *
 * Declares inliner logic.
 * Handles compilation optimizing
 * and compile time operations.
 *
 * -----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ------------- MACROS -----------------

// ...

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_ast_node);
FL_FORWARD_STRUCT(fl_operation);
FL_FORWARD_STRUCT(fl_cmp_context);

// ------------- GLOBALS ----------------

// ...

// ---------- DECLARATIONS --------------

fl_bool fl_inl_try_op_inline(fl_ast_node *id_node, fl_operation op, fl_cmp_context *cnt);

// ---------- DEFINITIONS ---------------

// ...
