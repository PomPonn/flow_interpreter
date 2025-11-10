#pragma once

#include "shared/common.h"

char *fl_file_read_text(const fl_char *filename);

void fl_file_free(fl_char *buffer);