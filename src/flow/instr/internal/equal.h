/**
 * ---------- equal.h ---------------------
 *
 * Defines equal instruction logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------------

#include "vecs/value_vec.h"

// ---------- DECLARATIONS ----------------

FL_INLINE_FN fl_value fl_instr_equal(fl_value_vec args);

// ---------- DEFINITIONS -----------------

FL_INLINE_FN fl_value fl_instr_equal(fl_value_vec args)
{
    fl_value left = args.data[0];
    fl_value right = args.data[1];

    assert(left.desc.type == FL_DATA_TYPE_INT);
    assert(right.desc.type == FL_DATA_TYPE_INT);

    fl_flow_int *l = left.data;
    fl_flow_int *r = left.data;

    fl_flow_bool *res = fl_new_bool(*l == *r);

    return fl_value_create(FL_DATA_TYPE_BOOL, res);
}