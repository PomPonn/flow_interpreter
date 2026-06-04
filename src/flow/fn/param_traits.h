/**
 * ---------- param_traits.h ----------
 *
 * Declares parameter traits logic.
 * -------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_param_traits
{
    FL_PARAM_TRAITS_NONE = 0b00000000,
    FL_PARAM_TRAITS_REF = 0b00000010,
    FL_PARAM_TRAITS_CONST = 0b00000100,
    // param indicates a call that is marked with SIMULATE trait
    FL_PARAM_TRAITS_SIMULATE = 0b00001000,
    // ???
    // FL_PARAM_TRAIT_EXPAND = 0b00000010,
};
typedef fl_byte fl_param_traits;