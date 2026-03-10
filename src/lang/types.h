#pragma once

#include "shared/common.h"
#include "structs/tok/token.h"

enum _fl_data_type
{
    FL_DATA_TYPE_NULL,
    FL_DATA_TYPE_INT,
    FL_DATA_TYPE_BOOL,
    FL_DATA_TYPE_FLOAT,
    FL_DATA_TYPE_ARRAY,
    FL_DATA_TYPE_OBJECT,
};
typedef fl_byte fl_data_type;

enum _fl_literal_type
{
    FL_AST_LITERAL_TYPE_STRING = FL_TOKEN_ALIAS_STRING,
    FL_AST_LITERAL_TYPE_NUMBER,
    FL_AST_LITERAL_TYPE_ARRAY,
};
typedef fl_byte fl_literal_type;