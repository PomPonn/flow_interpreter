/**
 * ---------- lib.h ----------------------
 *
 * Declares shared library loader logic.
 *
 * ---------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

typedef fl_unknown *fl_lib_handle;

// ---------- DECLARATIONS --------------

// loads shared library dynamically
// lib_name is without extension
// returns FL_NULL if link falis
fl_lib_handle fl_lib_load(const fl_char *lib_name);

fl_unknown *fl_lib_get_address(fl_lib_handle handle, const fl_char *fn_name);

fl_bool fl_lib_free(fl_lib_handle *handle);
