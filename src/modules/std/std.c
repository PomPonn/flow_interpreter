#include "std.h"

// ---- INCLUDES ----

#include "structs/call/call_param.h"
#include "structs/opr/opr_entry.h"
#include "lang/data_type.h"
#include "std/io/io.h"

// ---- DECLARATIONS ----

fl_unknown *_fl_std_add(fl_call_param x, fl_call_param y);

// ---- DEFINITIONS -----

fl_bool fl_std_register(fl_context *cnt)
{
    fl_obj_entry *std_object;
    fl_obj_entry_init(&std_object, "std");

    fl_id_tree_node *std = fl_id_tree_push_sibling(
        &cnt->id_tree,
        (fl_id_entry *)std_object);

    // fl_opr_entry *add_op;
    // fl_opr_entry_init(
    //     &add_op,
    //     "+",
    //     FL_FALSE,
    //     _fl_std_add,
    //     FL_DATA_TYPE_INT, );

    // fl_context_register_id(cnt, std, add_op);

    return fl_std_io_register(cnt, std);
}

fl_unknown *_fl_std_add(fl_call_param x, fl_call_param y)
{
}