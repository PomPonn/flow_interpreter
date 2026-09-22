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
#include "context/local_context.h"

// ---------- DECLARATIONS --------------

FL_INLINE_FN fl_value fl_instr_import(fl_value_vec args);

// ---------- DEFINITIONS ---------------

FL_INLINE_FN fl_value fl_instr_import(fl_value_vec args)
{
    fl_local_context *cnt = args.data[0].data;
    fl_char *import_str = args.data[1].data;

    fl_value imported = fl_mimp_import_id_value(import_str, cnt->base);
    return imported;
}