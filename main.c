/**
 * -----------------------------------
 *
 *      MAIN TESTING ENTRY POINT
 *
 * -----------------------------------
 */

#include "flow/globals.h"
#include "flow/flow.h"
#include "tools/profiler.h"

#include <stdio.h>

int main()
{
    fl_globals_init();

    fl_prof_profile(1000, 6, "lines", fl_flow_run(_TEST_INPUT_PATH));

    fl_globals_free();
}