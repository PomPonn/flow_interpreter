/**
 * ---------- assign.h --------------------
 *
 * Defines assign instruction logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/id/id_entry.h"
#include "vecs/value_vec.h"

// ---------- DECLARATIONS --------------

FL_SEALED_FN fl_value fl_instr_assign(fl_value_vec args);

// ---------- DEFINITIONS ---------------

FL_SEALED_FN fl_value fl_instr_assign(fl_value_vec args)
{
    fl_value ref = args.data[0];
    fl_value *what = &args.data[1];

    // WARNING: 'what' value is moved, not copied
    // it is probably unwanted behaviour
    // TODO: change this behaviour

    fl_id_entry *id = ref.data;

    // invalid data types
    if (!fl_bits_is_set(id->decl_type, what->desc.type))
        assert(FL_FALSE);

    fl_value_free(&id->value);
    id->value = *what;

    // clear 'what' value because it was moved
    *what = fl_value_empty();
    return fl_value_empty();
}