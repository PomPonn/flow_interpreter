/**
 * ---------- bitfield.h ----------------
 *
 * Declares bitfield logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- MACROS --------------------

#define fl_bitfield_is_set(bitfield, what) ((bitfield & (what)) == (what))

#define fl_bitfield_is_any(bitfield, of_what) (!(of_what) || bitfield & (of_what))

#define fl_bitfield_set(bitfield, value) bitfield |= (value)