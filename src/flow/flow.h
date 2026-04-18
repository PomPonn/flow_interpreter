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

/**
 * @brief Compiles source file pointed by `source_path` to byte code.
 * @param source_path Source file to compile.
 * @param context Context of compilation. Compiler reades & writes to this structure.
 */
fl_void fl_flow_compile(const fl_char *source_path, fl_context *cnt);