#pragma once

#include "shared/common.h"
#include "lang/data_type.h"
#include "struct/tok/token.h"

enum _fl_literal_type
{
    FL_LITERAL_TYPE_STRING = FL_TOKEN_ALIAS_LITERAL,
    FL_LITERAL_TYPE_INT,
    FL_LITERAL_TYPE_FLOAT,
    FL_LITERAL_TYPE_ARRAY,
};
typedef fl_byte fl_literal_type;

struct _fl_literal_info
{
    fl_literal_type literal_type;
    fl_unknown *data;
};
typedef struct _fl_literal_info fl_literal_info;

// copies data arg
fl_void fl_literal_info_init(fl_literal_info **info, fl_literal_type type, const fl_unknown *data);
fl_void fl_literal_info_free(fl_literal_info *info);
