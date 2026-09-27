/**
 * ---------- fn_traits.h ---------------
 *
 * Declares function traits logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "shared/bit.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_fn_traits
{
    // Nothing special.
    FL_FN_TRAITS_NONE = 0b00000000,

    // Function returns same result for same arguments (no side effects).
    FL_FN_TRAITS_PURE = 0b00000001,

    // Function directly alters flow context - receives base context as a first argument.
    FL_FN_TRAITS_META = 0b00000010,

    // Function is meant to be executed at compilation time.
    FL_FN_TRAITS_COMPILE_TIME = 0b00000100,
};

/**
 * @brief Function traits.
 */
typedef fl_byte fl_fn_traits;