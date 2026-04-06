#include "compiler/compiler.h"
#include "tools/profiler.h"
#include <stdio.h>

fl_int main()
{
    fl_static_predefs_init();

    fl_context cnt;
    fl_context_init(&cnt);

    fl_cmp_compile(_TEST_INPUT_PATH, &cnt);

    fl_context_free(&cnt);
    fl_static_predefs_free();
}