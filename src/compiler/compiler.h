/**
 * ---- compiler.h ----
 *
 * Declares compiler logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "lang/context.h"

// ---- TYPE DEFINITIONS ----

/**
 * @brief Represents byte code instructions.
 */
enum _fl_cmp_instruction
{
    // TODO: comment this when finished

    // arithmetic
    FL_INSTR_ADD,
    FL_INSTR_SUBS,
    FL_INSTR_MULT,
    FL_INSTR_DIV,
    FL_INSTR_POW,
    FL_INSTR_MOD,
    // ...
    FL_INSTR_CALL,
    FL_INSTR_GET,
    FL_INSTR_NEW,
    // use questioned
    FL_INSTR_STARTBLOCK,
    FL_INSTR_ENDBLOCK,
    FL_INSTR_STARTOP,
    FL_INSTR_ENDOP,

};

// ---- DECLARATIONS ----

/**
 * @brief Compiles source file pointed by `source_path` to byte code.
 * @param source_path Source file to compile.
 * @param context Context of compilation. Compiler reades & writes to this structure.
 */
fl_void fl_cmp_compile(fl_char *source_path, fl_context *context);