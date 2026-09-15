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
    FL_DATA_TYPE_NULL = 0b00000000,
    FL_DATA_TYPE_ANY = 0b11111111,
    FL_DATA_TYPE_SCOPE = 0b00000001,
    FL_DATA_TYPE_FN = 0b00000010,
    FL_DATA_TYPE_BOOL = 0b00000100,
    FL_DATA_TYPE_INT = 0b00001000,
    FL_DATA_TYPE_FLOAT = 0b00010000,
    FL_DATA_TYPE_ARRAY = 0b00100000,
    FL_DATA_TYPE_STRING = 0b01000000,
    FL_DATA_TYPE_CODE_PTR = 0b10000000,
    // increase fl_data_type size when adding more
};
typedef fl_byte fl_data_type;