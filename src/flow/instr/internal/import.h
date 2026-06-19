/**
 * ---------- import.h --------------------
 *
 * Defines import instruction logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "vecs/value_vec.h"
#include "tools/mod_importer.h"

// ---------- DECLARATIONS --------------

FL_FORWARD_STRUCT(fl_base_context);

FL_HEADER_FN fl_value fl_instr_import(fl_value_vec args);

// ---------- DEFINITIONS ---------------

// FL_HEADER_FN fl_value fl_instr_import(fl_base_context *cnt, fl_char *import_str);
FL_HEADER_FN fl_value fl_instr_import(fl_value_vec args)
{
    static_assert(FL_FALSE, "BAD ARGS");

    // fl_base_context *cnt = args.data;
    // fl_value imported = fl_mimp_import_id_value(import_str, cnt);
    // return imported;
}