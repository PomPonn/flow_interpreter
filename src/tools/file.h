#pragma once

#include "shared/common.h"

fl_char *fl_file_read(const fl_char *filename);

fl_void fl_file_append(const fl_char *filename, const fl_char *content);

FL_HEADER_FN fl_void fl_file_free(fl_char **buffer)
{
    fl_mem_free(*buffer);
    buffer = FL_NULL;
}