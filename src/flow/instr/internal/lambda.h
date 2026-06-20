/**
 * ---------- lambda.h --------------------
 *
 * Defines lambda instruction logic.
 *
 * ----------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/id/id_entry.h"
#include "vecs/value_vec.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_value fl_instr_lambda(fl_value_vec args);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_value fl_instr_lambda(fl_value_vec args)
{
    return FL_VALUE_EMPTY;

    fl_value params_arr = args.data[0];
    fl_value lambda_body = args.data[1];
}