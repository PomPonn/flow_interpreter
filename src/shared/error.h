#pragma once

#include "shared/common.h"
#include "shared/token.h"

enum _fl_err_code
{
    FL_ERR_NONE = 0,
    // 4** - abstract syntax tree codes
    FL_ERR_AST_SYNTAX = 401,
};
typedef fl_uint fl_err_code;

struct _fl_err_record
{
    fl_err_code code;
    fl_size line;
    fl_size column;
    fl_char *token;
};
typedef struct _fl_err_record fl_err_record;

fl_bool fl_err_report(fl_err_code code, fl_token bad_token);

void fl_err_clear();

void fl_err_execute();