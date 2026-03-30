#include "compiler/compiler.h"
#include "tools/profiler.h"
#include "tools/byte_reader.h"
#include <stdio.h>

fl_int main()
{
    fl_context cnt;
    fl_context_init(&cnt);

    fl_context_load_module_by_str(&cnt, "meta");

    fl_cmp_compile(_TEST_INPUT_PATH, &cnt);
}