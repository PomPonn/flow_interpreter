
#include "base_context.h"

fl_void fl_base_context_init(fl_base_context *cnt)
{
    fl_lib_handle_vec_init(&cnt->lib_handles);
    fl_cstr_vec_init(&cnt->lib_names);
    fl_value_vec_init(&cnt->literals);
    fl_scope_init(&cnt->scope, FL_NULL);
}

fl_void fl_base_context_free(fl_base_context *cnt)
{
    fl_lib_handle_vec_free(&cnt->lib_handles);
    fl_cstr_vec_free(&cnt->lib_names);
    fl_value_vec_free(&cnt->literals);

    fl_scope_free(&cnt->scope);
    fl_mem_free(cnt->scope);
}
