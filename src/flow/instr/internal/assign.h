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

FL_HEADER_FN fl_value fl_instr_assign(fl_value_vec args);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_value fl_instr_assign(fl_value_vec args)
{
    fl_value ref = args.data[0];
    fl_value *what = &args.data[1];

    // WARNING: 'what' value is moved, not copied
    // it is probably unwanted behaviour
    // TODO: change this behaviour

    fl_value *val = ref.data;

    if (!fl_value_is_desc_by(*val, what->desc))
        // invalid data descriptors
        assert(FL_FALSE);

    fl_value_free(val);
    *val = *what;

    // clear 'what' value because it was moved
    *what = fl_value_empty();
    return fl_value_empty();
}