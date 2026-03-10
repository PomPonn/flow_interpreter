#pragma once

#include "shared/common.h"
#include "structs/id/id_tree.h"
#include "structs/id/id_list.h"
#include "structs/obj/obj_entry.h"
#include "structs/byte_code.h"

struct _fl_context
{
    fl_id_tree_node *id_tree;
    fl_id_list id_list;
    fl_byte_code byte_code;
};
typedef struct _fl_context fl_context;

fl_void fl_context_init(fl_context *cnt);

fl_void fl_context_register_id(fl_context *cnt, fl_id_tree_node *parent, fl_id_entry *entry);

fl_void fl_context_free(fl_context *cnt);
