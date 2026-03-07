#include "std.h"

#include "std/io/io.h"

fl_void fl_std_register(fl_context *cnt)
{
    fl_std_io_register(cnt);
}