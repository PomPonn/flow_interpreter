#pragma once

#include "shared/common.h"
#include "struct/info/file_location_info.h"

enum _fl_err_type
{
    FL_ERR_NONE,
    FL_ERR_FILE,
    FL_ERR_AST,
    FL_ERR_COMPILATION,
    FL_ERR_IMPORT,
};
typedef fl_ushort fl_err_type;

struct _fl_err_record
{
    fl_err_type type;
    fl_file_location_info location;
    const fl_char *message;
    fl_char *near;
};
typedef struct _fl_err_record fl_err_record;

fl_void fl_err_report(fl_err_type type, const fl_char *message, const fl_char *near, fl_file_location_info location);

fl_void fl_err_clear();

fl_void fl_err_throw(fl_err_type type, const fl_char *message, const fl_char *near, fl_file_location_info location);

fl_void fl_err_execute();
