#pragma once

#include "shared/common.h"

enum _fl_data_type
{
    FL_DATA_TYPE_NULL = 0b00000000,
    FL_DATA_TYPE_OBJECT = 0b00000001,
    FL_DATA_TYPE_CALLABLE = 0b00000010,
    FL_DATA_TYPE_BOOL = 0b00000100,
    FL_DATA_TYPE_INT = 0b00001000,
    FL_DATA_TYPE_FLOAT = 0b00010000,
    FL_DATA_TYPE_ARRAY = 0b00100000,
    FL_DATA_TYPE_REF = 0b01000000,
};
typedef fl_byte fl_data_type;
typedef fl_byte fl_data_type_mix;

inline fl_bool fl_data_type_mix_has(fl_data_type_mix mix, fl_data_type_mix types)
{
    return (mix & types) == types;
}
