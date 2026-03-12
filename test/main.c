#include "compiler/compiler.h"

#include <stdio.h>

int main()
{
    fl_context cnt;
    fl_context_init(&cnt);

    fl_context_load_module_str(&cnt, "meta");

    fl_comp_compile(_TEST_INPUT_PATH, &cnt);

    return 0;
}