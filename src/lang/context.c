#include "context.h"

fl_void fl_context_init(fl_context *cnt)
{
    cnt->id_tree = FL_NULL;
    fl_byte_code_init(&cnt->byte_code, 0);
}

fl_void fl_context_free(fl_context *cnt)
{
    fl_byte_code_free(&cnt->byte_code);
    fl_id_tree_node_free(cnt->id_tree);
    cnt->id_tree = FL_NULL;
}

fl_size fl_context_next_id_table_index(fl_context *cnt)
{
    return cnt->next_id_table_index++;
}