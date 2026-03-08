#include "std.h"

#include "std/io/io.h"

fl_void fl_std_register(fl_context *cnt)
{
    fl_obj_entry *std_object;
    fl_obj_entry_init(&std_object, "std");

    fl_id_tree_node *std = fl_id_tree_push_sibling(
        &cnt->id_tree,
        (fl_id_entry *)std_object);

    fl_std_io_register(cnt, std);
}