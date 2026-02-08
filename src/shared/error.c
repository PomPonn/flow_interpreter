#include "error.h"

#include "shared/memory.h"
#include "shared/token.h"

#include <stdio.h>
#include <stdlib.h>

// --- DEFINES ---

#define _MAX_ERROR_NUM 32

// --- GLOBALS ---

fl_err_record _FL_ERRORS[_MAX_ERROR_NUM];
fl_size _FL_ERRORS_NUM = 0;

// --- STATIC FUNCTION DEFINITIONS ---

static inline const fl_char *_fl_err_type_to_string(fl_err_type type);

// --- IMPLEMENTATIONS ---

static inline const fl_char *_fl_err_type_to_string(fl_err_type type)
{
    switch (type)
    {
    case FL_ERR_NONE:
        return "No error";
    case FL_ERR_AST:
        return "Invalid syntax";
    default:
        return "Undefined error";
    }
}

// reports error but doesn't end interpretation (so we can catch more errors)
// exception occurs when there is no room for more errors. Than it throws;
fl_void fl_err_report(fl_err_type type, const fl_char *message, fl_token bad_token)
{
    if (_FL_ERRORS_NUM + 1 > _MAX_ERROR_NUM)
    {
        fl_err_execute();
        return;
    }

    _FL_ERRORS[_FL_ERRORS_NUM] = (fl_err_record){
        .type = type,
        .description = message,
        .column = bad_token.column,
        .line = bad_token.line,
        .token = fl_str_dup(bad_token.value)};

    _FL_ERRORS_NUM++;
}

fl_void fl_err_clear()
{
    if (_FL_ERRORS_NUM == 0)
        return;

    for (fl_size i = 0; i < _FL_ERRORS_NUM; i++)
    {
        fl_mem_free(_FL_ERRORS[i].token);
    }

    _FL_ERRORS_NUM = 0;
}

// reports an error and ends interpretation
fl_void fl_err_throw(fl_err_type type, const fl_char *message, fl_token bad_token)
{
#if !_SUPPRESS_ERRORS
    fl_err_report(type, message, bad_token);
    fl_err_execute();
#endif
}

fl_void fl_err_execute()
{
    for (fl_size i = 0; i < _FL_ERRORS_NUM; i++)
    {
        printf(
            "Error in line %lu, column %lu near token '%s':\n%s: %s\n",
            _FL_ERRORS[i].line + 1,
            _FL_ERRORS[i].column + 1,
            fl_token_as_printable(_FL_ERRORS[i].token),
            _fl_err_type_to_string(_FL_ERRORS[i].type),
            _FL_ERRORS[i].description);
    }

    // TODO: free program's allocated memory here

    fl_err_clear();
    exit(-1);
}