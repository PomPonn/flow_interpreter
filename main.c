#include "compiler/compiler.h"
#include "tools/profiler.h"
#include <stdio.h>

fl_int main()
{
    fl_context cnt;
    fl_context_init(&cnt);

    fl_predef_init_internal();

    fl_predef pd = fl_predef_create(FL_PREDEF_TYPE_ADD, fl_str_dup("+"), FL_TRUE);
    fl_predef_vec_push(&cnt.predefines, pd);
    pd = fl_predef_create(FL_PREDEF_TYPE_IMPORT, fl_str_dup("import"), FL_FALSE);
    fl_predef_vec_push(&cnt.predefines, pd);
    pd = fl_predef_create(FL_PREDEF_TYPE_ASSIGN, fl_str_dup("="), FL_TRUE);
    fl_predef_vec_push(&cnt.predefines, pd);

    fl_context_load_module_by_str(&cnt, "meta");

    fl_cmp_compile(_TEST_INPUT_PATH, &cnt);

    fl_predef_free_internal();
}