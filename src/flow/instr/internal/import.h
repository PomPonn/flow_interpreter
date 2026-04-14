/**
 * ---------- import.h --------------------
 *
 * Defines import instruction logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/id/id_entry.h"
#include "tools/mod_importer.h"

// ---------- TYPE DEFINITIONS ----------

// forward typedef
struct _fl_context;
typedef struct _fl_context fl_context;

FL_HEADER_FN fl_value fl_instr_import(fl_context *cnt, fl_char *import_str);

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_value fl_instr_import(fl_context *cnt, fl_char *import_str)
{
    fl_value imported = fl_mimp_import_id_value(cnt, import_str);
    return imported;
}