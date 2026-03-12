#ifdef _WIN32
#include "lib.h"

#include <windows.h>
#include <stdio.h>

fl_lib_handle fl_lib_load(const fl_char *lib_name)
{
    fl_char buffer[_FL_MAX_LIB_NAME_LEN];
    sprintf(buffer, "%s.dll", lib_name);
    return LoadLibrary(lib_name);
}

fl_bool fl_lib_free(fl_lib_handle handle)
{
    return FreeLibrary(handle);
}

#endif