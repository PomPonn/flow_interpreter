/**
 * ---------- flow.h --------------------
 *
 * Declares main flow language logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/globals.h"
#include "flow/context.h"

// ---------- DECLARATIONS --------------

fl_void fl_flow_run(const fl_char *source_path, fl_context *cnt);