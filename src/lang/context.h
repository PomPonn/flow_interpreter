#pragma once

#include "shared/common.h"
#include "struct/id/id_tree.h"
#include "struct/id/id_list.h"
#include "struct/obj/obj_entry.h"
#include "struct/byte_code.h"

struct _fl_context
{
    fl_id_tree_node *id_tree;
    fl_id_list id_list;
    fl_byte_code byte_code;
};
typedef struct _fl_context fl_context;

fl_void fl_context_init(fl_context *cnt);

fl_int fl_context_resolve_id(fl_context *cnt, fl_id_chain *id_chain, const fl_id_entry **id_value);

fl_void fl_context_register_id(fl_context *cnt, fl_id_tree_node *parent, fl_id_entry *entry);

fl_void fl_context_load_module(fl_context *cnt, fl_id_chain *id_chain);

fl_void fl_context_free(fl_context *cnt);
