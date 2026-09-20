/**
 * ---------- not_equal.h ---------------------
 *
 * Defines not equal instruction logic.
 *
 * --------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------------

#include "vecs/value_vec.h"

// ---------- DECLARATIONS ----------------

FL_INLINE_FN fl_value fl_instr_not_equal(fl_value_vec args);

// ---------- DEFINITIONS -----------------

FL_INLINE_FN fl_value fl_instr_not_equal(fl_value_vec args)
{
    fl_value left = args.data[0];
    fl_value right = args.data[1];

    return fl_value_empty();
}