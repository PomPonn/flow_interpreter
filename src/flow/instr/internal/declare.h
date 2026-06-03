/**
 * ---------- assign.h --------------------
 *
 * Defines assign instruction logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/id/id_entry.h"
#include "flow/value.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_value fl_instr_declare(fl_value null_id_ref, fl_value type);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_value fl_instr_declare(fl_value null_id_ref, fl_value type)
{
    fl_id_entry *id = (fl_id_entry *)null_id_ref.data;

    // ...

    return id->value;
}