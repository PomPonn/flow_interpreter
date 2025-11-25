#include "error.h"

#include <stdio.h>
#include <stdlib.h>

#include "shared/memory.h"
#include "shared/token.h"

// --- DEFINES ---

#define SUPPRESS_ERRORS

#define _MAX_ERROR_NUM 32

// --- GLOBALS ---

fl_err_record _FL_ERRORS[_MAX_ERROR_NUM];
fl_size _FL_ERRORS_NUM = 0;

// --- STATIC FUNCTIONS DEFINITION ---

static const fl_char *_fl_err_code_to_message(fl_err_code code);

// --- IMPLEMENTATIONS ---

static const fl_char *_fl_err_code_to_message(fl_err_code code)
{
    switch (code)
    {
    case FL_ERR_NONE:
        return "No error";
    case FL_ERR_AST_SYNTAX:
        return "Invalid syntax";
    default:
        return "Undefined error";
    }
}

fl_bool fl_err_report(fl_err_code code, fl_token bad_token)
{
    if (_FL_ERRORS_NUM + 1 > _MAX_ERROR_NUM)
    {
        return FL_FALSE;
    }

    _FL_ERRORS[_FL_ERRORS_NUM] = (fl_err_record){
        .code = code,
        .column = bad_token.column,
        .line = bad_token.line,
        .token = fl_str_dup(bad_token.value)};

    _FL_ERRORS_NUM++;

    return FL_TRUE;
}

void fl_err_clear()
{
    if (!_FL_ERRORS_NUM)
        return;

    for (fl_size i = 0; i < _FL_ERRORS_NUM; i++)
    {
        fl_mem_free(_FL_ERRORS[i].token);
    }

    _FL_ERRORS_NUM = 0;
}

void fl_err_throw(fl_err_code code, fl_token bad_token)
{
#ifndef SUPPRESS_ERRORS
    fl_err_report(code, bad_token);

    fl_err_execute();

    fl_err_clear();
    exit(-1);
#endif
}

void fl_err_execute()
{
    for (fl_size i = 0; i < _FL_ERRORS_NUM; i++)
    {
        printf(
            "Error in line %zu, column %zu near token '%s':\n%s\n",
            _FL_ERRORS[i].line,
            _FL_ERRORS[i].column,
            fl_token_str_as_printable(_FL_ERRORS[i].token),
            _fl_err_code_to_message(_FL_ERRORS[i].code));
    }

    fl_err_clear();
    exit(-1);
}