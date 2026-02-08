#pragma once

#include "shared/common.h"
#include "shared/token.h"

enum _fl_err_type
{
    FL_ERR_NONE,
    FL_ERR_FILE,
    FL_ERR_TOKENIZER,
    FL_ERR_AST,
};
typedef fl_ushort fl_err_type;

struct _fl_err_record
{
    fl_err_type type;
    const fl_char *description;
    fl_size line;
    fl_size column;
    fl_char *token;
};
typedef struct _fl_err_record fl_err_record;

fl_void fl_err_report(fl_err_type code, const fl_char *message, fl_token bad_token);

fl_void fl_err_clear();

fl_void fl_err_throw(fl_err_type code, const fl_char *message, fl_token bad_token);

fl_void fl_err_execute();