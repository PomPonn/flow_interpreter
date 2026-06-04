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

#define fl_bitfield_get(bitfield, what) (bitfield & (what))

#define fl_bitfield_set(bitfield, value) bitfield |= (value)