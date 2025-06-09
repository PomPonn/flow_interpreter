#pragma once

#include "common.h"

char *fl_read_file_as_str(const char *filename);

void fl_free_file(uint8_t *buffer);