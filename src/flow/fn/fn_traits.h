/**
 * ---------- fn_traits.h --------------------
 *
 * Declares function traits logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "shared/bitfield.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_fn_traits
{
    FL_FN_TRAITS_NONE = 0b00000000,
    FL_FN_TRAITS_PURE = 0b00000001,
    FL_FN_TRAITS_SIMULATE = 0b00000010,
};
typedef fl_byte fl_fn_traits;