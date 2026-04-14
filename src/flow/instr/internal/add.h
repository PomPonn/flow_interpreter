/**
 * ---------- add.h --------------------
 *
 * Defines add instruction logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/value.h"

// ---------- MACROS --------------------

#define _FL_INT_VAL(v) *(fl_flow_int *)v.data
#define _FL_FLOAT_VAL(v) *(fl_flow_int *)v.data

// ---------- TYPE DEFINITIONS ----------

FL_HEADER_FN fl_value fl_instr_add(fl_value left, fl_value right);

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_value fl_instr_add(fl_value left, fl_value right)
{
    // unsave conversion
    // TODO: implement type conversion system

    if (left.data_type == FL_DATA_TYPE_INT)
    {
        fl_flow_int *res = fl_mem_alloc(sizeof(fl_flow_int));
        *res = _FL_INT_VAL(left) + _FL_INT_VAL(right);
        return fl_value_create(FL_DATA_TYPE_INT, res);
    }
    else if (left.data_type == FL_DATA_TYPE_FLOAT)
    {
        fl_flow_float *res = fl_mem_alloc(sizeof(fl_flow_float));
        *res = _FL_FLOAT_VAL(left) + _FL_FLOAT_VAL(right);
        return fl_value_create(FL_DATA_TYPE_FLOAT, res);
    }
    else
    {
        assert(FL_FALSE);
        return FL_VALUE_EMPTY;
    }
}