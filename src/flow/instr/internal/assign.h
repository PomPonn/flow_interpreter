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

FL_HEADER_FN fl_value fl_instr_assign(fl_value id_ref, fl_value *what);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_value fl_instr_assign(fl_value id_ref, fl_value *what)
{
    // WARNING: 'what' value is moved, not copied
    // it is probably unwanted behaviour
    // TODO: change this behaviour

    fl_id_entry *id = (fl_id_entry *)id_ref.data;

    if (!fl_bitfield_is_set(id->decl_type, what->data_type))
        // invalid data type
        assert(FL_FALSE);

    fl_value_free(&id->value);
    id->value = *what;

    // clear 'what' value because it was moved
    *what = FL_VALUE_EMPTY;

    return id->value;
}