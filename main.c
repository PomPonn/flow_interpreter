#include "flow/flow.h"
#include "tools/profiler.h"
#include <stdio.h>

fl_int main()
{
    fl_globals_init();

    fl_context cnt;
    fl_context_init(&cnt);
    fl_globals_static_predefs_to_context(&cnt);

    fl_flow_run(_TEST_INPUT_PATH, &cnt);

    fl_context_free(&cnt);
    fl_globals_free();
}