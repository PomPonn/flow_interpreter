/**
 * ---------- bitfield.h --------------------
 *
 * Declares bitfield logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_bitfield8_has(fl_byte bitfield, fl_byte what);

// ---------- DEFINITIONS -----------

FL_HEADER_FN fl_bitfield8_has(fl_byte bitfield, fl_byte what)
{
    return (bitfield & what) == what;
}
