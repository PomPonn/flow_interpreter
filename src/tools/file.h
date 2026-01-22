#pragma once

#include "shared/common.h"

char *fl_file_read(const fl_char *filename);

void fl_file_append(const fl_char *filename, char *data);

fl_void fl_file_free(fl_char *buffer);