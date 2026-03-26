#pragma once

#include "shared/common.h"
#include "lang/data_type.h"
#include "tokenizer/token.h"

enum _fl_ast_literal_type
{
    FL_LITERAL_TYPE_STRING = FL_TOKEN_ALIAS_LITERAL,
    FL_LITERAL_TYPE_INT,
    FL_LITERAL_TYPE_FLOAT,
    FL_LITERAL_TYPE_ARRAY,
};
typedef fl_byte fl_ast_literal_type;

struct _fl_ast_literal_info
{
    fl_ast_literal_type ast_literal_type;
    fl_unknown *data;
};
typedef struct _fl_ast_literal_info fl_ast_literal_info;

// copies data arg
fl_void fl_ast_literal_info_init(fl_ast_literal_info **info, fl_ast_literal_type type, const fl_unknown *data);

FL_HEADER_FN fl_void fl_ast_literal_info_free(fl_ast_literal_info **info)
{
    fl_ast_literal_info *ptr = *info;
    fl_mem_free(ptr->data);
    fl_mem_free(ptr);
    *info = FL_NULL;
}
