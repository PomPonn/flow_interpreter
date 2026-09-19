/**
 * -----------------------------------
 *
 *      MAIN TESTING ENTRY POINT
 *
 * -----------------------------------
 */

#include "flow/globals.h"
#include "flow/flow.h"
#include "shared/str.h"

#include <stdio.h>

#define MAX_FILE_PATH_SIZE 2048

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Missing sample file path (relative to 'samples' directory)");
        return 0;
    }

    // First arg is a program name, so take the second arg
    char file_path[MAX_FILE_PATH_SIZE] = _SAMPLE_DIR "/";

    fl_str_concat(file_path, argv[1], sizeof(file_path));

    fl_globals_init();

    fl_prof_profile(1000, 6, "lines", fl_flow_run(file_path));

    fl_globals_free();
}