/**
 * ---------- data_type.h ---------------
 *
 * Declares data types.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "shared/bitfield.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_data_type
{
    FL_DATA_TYPE_NULL = 0b000000000,
    FL_DATA_TYPE_ANY = 0b011111111,
    FL_DATA_TYPE_SCOPE = 0b000000001,
    FL_DATA_TYPE_FN = 0b000000010,
    FL_DATA_TYPE_BOOL = 0b000000100,
    FL_DATA_TYPE_INT = 0b000001000,
    FL_DATA_TYPE_FLOAT = 0b000010000,
    FL_DATA_TYPE_ARRAY = 0b000100000,
    FL_DATA_TYPE_STRING = 0b001000000,
    FL_DATA_TYPE_CODE_PTR = 0b010000000,
};
typedef fl_ushort fl_data_type;