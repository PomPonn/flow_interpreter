/**
 * ---- compiler.h ----
 *
 * Declares compiler logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "lang/context.h"

// ---- DECLARATIONS ----

/**
 * @brief Compiles source file pointed by `source_path` to byte code.
 * @param source_path Source file to compile.
 * @param context Context of compilation. Compiler reades & writes to this structure.
 */
fl_void fl_cmp_compile(fl_char *source_path, fl_context *context);