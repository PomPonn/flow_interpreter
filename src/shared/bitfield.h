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

// ---------- MACROS --------------------

#define fl_bitfield_has(bitfield, what) ((bitfield & what) == what)
