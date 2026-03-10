#include "context.h"

// ---- DEFINITIONS ----

fl_void fl_context_init(fl_context *cnt)
{
    cnt->id_tree = FL_NULL;
    fl_byte_code_init(&cnt->byte_code, 0);
    fl_id_list_init(&cnt->id_list, 0);
}

fl_void fl_context_free(fl_context *cnt)
{
    fl_byte_code_free(&cnt->byte_code);
    fl_id_list_free(&cnt->id_list);
    fl_id_tree_node_free(cnt->id_tree);
    cnt->id_tree = FL_NULL;
}

fl_void fl_context_register_id(fl_context *cnt, fl_id_tree_node *parent, fl_id_entry *entry)
{
    // TODO: handle push fail
    fl_id_tree_push_child(parent, entry);
    fl_id_list_push(&cnt->id_list, entry);
}