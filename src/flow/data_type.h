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
#include "shared/bit.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_data_type
{
    // Empty data type.
    FL_DATA_TYPE_NULL = 0b00000000,

    // Any data type.
    FL_DATA_TYPE_ANY = 0b11111111,

    // Scope data type.
    FL_DATA_TYPE_SCOPE = 0b00000001,

    // Function data type.
    FL_DATA_TYPE_FN = 0b00000010,

    // Bool data type.
    FL_DATA_TYPE_BOOL = 0b00000100,

    // Integer data type.
    FL_DATA_TYPE_INT = 0b00001000,

    // Float data type.
    FL_DATA_TYPE_FLOAT = 0b00010000,

    // Array data type.
    FL_DATA_TYPE_ARRAY = 0b00100000,

    // String data type.
    FL_DATA_TYPE_STRING = 0b01000000,

    // Code segment data type.
    FL_DATA_TYPE_CODE_SEGM = 0b10000000,
};

/**
 * @brief Data type.
 */
typedef fl_byte fl_data_type;
