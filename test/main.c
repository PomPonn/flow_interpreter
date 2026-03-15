#include "compiler/compiler.h"

int main()
{
    fl_context cnt;
    fl_context_init(&cnt);

    fl_id_node *meta = fl_context_load_module_by_str(&cnt, "meta");
    fl_context_add_global_scope(&cnt, meta);

    fl_comp_compile(_TEST_INPUT_PATH, &cnt);
}