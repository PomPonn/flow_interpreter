#include "error.h"

#include "shared/memory.h"
#include "shared/str.h"

#include <stdio.h>
#include <stdlib.h>

// ---------- MACROS ----------

#define _MAX_ERROR_NUM 32

// ---------- GLOBALS ----------

fl_err_record _g_errors[_MAX_ERROR_NUM];
fl_size _g_errors_num = 0;

// ---------- DEFINITIONS ----------

// reports error but doesn't end interpretation (so we can catch more errors)
// exception occurs when there is no room for more errors. Than it throws;
fl_void fl_err_report(fl_err_type type, const fl_char *message, const fl_char *near, fl_token_location location)
{
    if (_g_errors_num + 1 > _MAX_ERROR_NUM)
        // execute errors if current one exceeds the limit
        fl_err_execute();

    _g_errors[_g_errors_num] = (fl_err_record){
        .type = type,
        .message = message,
        .location = location,
        .near = fl_str_dup(near)};

    _g_errors_num++;
}

fl_void fl_err_clear()
{
    if (_g_errors_num == 0)
        return;

    for (fl_size i = 0; i < _g_errors_num; i++)
        fl_mem_free(_g_errors[i].near);

    _g_errors_num = 0;
}

// reports an error and ends interpretation
fl_void fl_err_throw(fl_err_type type, const fl_char *message, const fl_char *near, fl_token_location location)
{
#if !_SUPPRESS_ERRORS
    fl_err_report(type, message, near, location);
    fl_err_execute();
#endif
}

fl_void fl_err_execute()
{
    if (_g_errors_num == 0)
        return;

    for (fl_size i = 0; i < _g_errors_num; i++)
    {
        switch (_g_errors[i].type)
        {
        case FL_ERR_FILE:
            printf(
                "[ERROR] File error while processing file '%s':\n%s\n",
                _g_errors[i].location.description,
                _g_errors[i].message);
            break;
        case FL_ERR_AST:
            printf(
                "[ERROR] Invalid syntax in '%s'\n"
                "line %u, column %u near '%s':\n%s\n",
                _g_errors[i].location.description,
                _g_errors[i].location.line + 1,
                _g_errors[i].location.column + 1,
                _g_errors[i].near,
                _g_errors[i].message);
            break;
        case FL_ERR_COMPILATION:
            printf(
                "[ERROR] Compilation failed while processing '%s'\n"
                "line %u, column %u near '%s':\n%s\n",
                _g_errors[i].location.description,
                _g_errors[i].location.line + 1,
                _g_errors[i].location.column + 1,
                _g_errors[i].near,
                _g_errors[i].message);
            break;
        case FL_ERR_IMPORT:
            printf(
                "[ERROR] Failed to import '%s' while processing '%s':\n%s",
                _g_errors[i].near,
                _g_errors[i].location.description,
                _g_errors[i].message);
            break;
        default:
            // unhandled error type
            assert(FL_FALSE);
            break;
        }
    }

    // TODO: free program's allocated memory here

    fl_err_clear();
    exit(-1);
}
