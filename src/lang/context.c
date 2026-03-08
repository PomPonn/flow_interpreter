#include "context.h"

// ---- DECLARATIONS ----

// ---- DEFINITIONS ----

fl_void fl_context_init(fl_context *cnt)
{
    cnt->id_tree = FL_NULL;
    fl_byte_code_init(&cnt->byte_code, 0);
    fl_fn_list_init(&cnt->func_list, 0);
}

fl_void fl_context_free(fl_context *cnt)
{
    fl_byte_code_free(&cnt->byte_code);
    fl_fn_list_free(&cnt->func_list);
    fl_id_tree_node_free(cnt->id_tree);
    cnt->id_tree = FL_NULL;
}

fl_void fl_context_register_func(fl_context *cnt, fl_id_tree_node *parent, fl_fn_entry *entry)
{
    // TODO: handle push fail
    fl_id_tree_push_child(parent, (fl_id_entry *)entry);
    fl_fn_list_push(&cnt->func_list, entry);
}

fl_void fl_context_register_obj(fl_context *cnt, fl_id_tree_node *parent, fl_obj_entry *entry)
{
    assert(FL_FALSE);
}