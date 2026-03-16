#pragma once

#include "shared/common.h"
#include "struct/id/id_node_vec.h"
#include "struct/id/obj_id_entry.h"
#include "struct/byte_vec.h"
#include "struct/lib_handle_vec.h"

struct _fl_context
{
    fl_id_node_vec global_scopes;
    fl_lib_handle_vec lib_handles;
    fl_id_node *id_tree;
    fl_id_node_vec id_list;
    fl_byte_vec byte_vec;
};
typedef struct _fl_context fl_context;

fl_void fl_context_init(fl_context *cnt);

fl_int fl_context_resolve_id(
    fl_context *cnt,
    fl_id_node *scope,
    fl_id_chain *id_chain,
    const fl_id_node **id_node);

fl_id_node *fl_context_register_id(fl_context *cnt, fl_id_node *parent, fl_id_entry *entry);

fl_id_node *fl_context_load_module_by_id(fl_context *cnt, fl_id_chain *id_chain);

fl_id_node *fl_context_load_module_by_str(fl_context *cnt, const fl_char *id_str);

fl_bool fl_context_add_global_scope(fl_context *cnt, fl_id_node *id_node);

fl_void
fl_context_free(fl_context *cnt);
