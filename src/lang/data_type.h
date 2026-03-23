#pragma once

#include "shared/common.h"

enum _fl_data_type
{
    FL_DATA_TYPE_NULL = 0b00000000,
    FL_DATA_TYPE_ANY = 0b11111111,
    FL_DATA_TYPE_SCOPE = 0b00000001,
    FL_DATA_TYPE_CALLABLE = 0b00000010,
    FL_DATA_TYPE_BOOL = 0b00000100,
    FL_DATA_TYPE_NUMBER = 0b00001000,
    FL_DATA_TYPE_ARRAY = 0b00010000,
    FL_DATA_TYPE_STRING = 0b00100000,
};
typedef fl_byte fl_data_type;

FL_HEADER_FN fl_bool fl_data_type_has(fl_data_type type, fl_data_type what)
{
    return (type & what) == what;
}
