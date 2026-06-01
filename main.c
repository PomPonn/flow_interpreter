#include "flow/globals.h"
#include "flow/flow.h"
#include "tools/profiler.h"
#include <stdio.h>

fl_int main()
{
    fl_globals_init();

    fl_flow_run(_TEST_INPUT_PATH);

    fl_globals_free();
}