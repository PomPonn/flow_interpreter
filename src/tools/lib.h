#pragma once

#include "shared/common.h"

#define _FL_MAX_LIB_NAME_LEN 128

typedef fl_unknown *fl_lib_handle;

// loads shared library dynamically
// lib_name is without extension
// returns FL_NULL if link falis
fl_lib_handle fl_lib_load(const fl_char *lib_name);

fl_bool fl_lib_free(fl_lib_handle handle);