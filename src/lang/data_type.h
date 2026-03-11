#pragma once

#include "shared/common.h"
#include "structs/tok/token.h"

enum _fl_data_type
{
    FL_DATA_TYPE_NULL = 0b00000000,
    FL_DATA_TYPE_OBJECT = 0b00000001,
    FL_DATA_TYPE_CALLABLE = 0b00000010,
    FL_DATA_TYPE_BOOL = 0b00000100,
    FL_DATA_TYPE_INT = 0b00001000,
    FL_DATA_TYPE_FLOAT = 0b00010000,
    FL_DATA_TYPE_ARRAY = 0b00100000,
};
typedef fl_byte fl_data_type;
typedef fl_byte fl_data_type_mix;

enum _fl_literal_type
{
    FL_AST_LITERAL_TYPE_STRING = FL_TOKEN_ALIAS_STRING,
    FL_AST_LITERAL_TYPE_NUMBER,
    FL_AST_LITERAL_TYPE_ARRAY,
};
typedef fl_byte fl_literal_type;

fl_bool fl_data_type_mix_has(fl_data_type_mix mix, fl_data_type_mix types)
{
    return (mix & types) == types;
}