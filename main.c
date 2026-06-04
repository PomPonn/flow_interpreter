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
    fl_prof_profile(1, 1, "run", {
        fl_globals_init();

        fl_flow_run(_TEST_INPUT_PATH);

        fl_globals_free();
    })
}