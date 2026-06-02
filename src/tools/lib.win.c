/**
 * ---------- lib.win.c --------------------
 *
 * Implements shared library loader logic
 * for Windows platform
 *
 * -----------------------------------------
 */

#ifdef _WIN32

#include "lib.h"

// ---------- DEPENDENCIES --------------

#include <windows.h>
#include <stdio.h>

// ------------- MACROS -----------------

#define _FL_MAX_LIB_NAME_LEN 128

// ---------- DEFINITIONS ---------------

fl_lib_handle fl_lib_load(const fl_char *lib_name)
{
    fl_char buffer[_FL_MAX_LIB_NAME_LEN];
    sprintf(buffer, "%s.dll", lib_name);
    return LoadLibrary(lib_name);
}

fl_unknown *fl_lib_get_address(fl_lib_handle handle, const fl_char *fn_name)
{
    return GetProcAddress(handle, fn_name);
}

fl_bool fl_lib_free(fl_lib_handle *handle)
{
    fl_bool res = FreeLibrary(*handle);
    *handle = FL_NULL;
    return res;
}

#endif
