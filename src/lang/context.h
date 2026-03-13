#pragma once

#include "shared/common.h"
#include "struct/id/id_node.h"
#include "struct/id/id_node_vec.h"
#include "struct/obj/obj_id_entry.h"
#include "struct/byte_code.h"
#include "struct/lib/lib_handle_vec.h"

#define fl_context_load_module_str(cnt_ptr, id_str, global_scope) \
    {                                                             \
        fl_id_chain _chain;                                       \
        fl_id_chain_init_parse(&_chain, id_str);                  \
        fl_context_load_module(cnt_ptr, &_chain, global_scope);   \
        fl_id_chain_free(&_chain);                                \
    }

struct _fl_context
{
    fl_id_node_vec global_scopes;
    fl_lib_handle_vec lib_handles;
    fl_id_node *id_tree;
    fl_id_node_vec id_list;
    fl_byte_code byte_code;
};
typedef struct _fl_context fl_context;

fl_void fl_context_init(fl_context *cnt);

fl_int fl_context_resolve_id(fl_context *cnt, fl_id_chain *id_chain, const fl_id_node **id_node);

fl_id_node *fl_context_register_id(fl_context *cnt, fl_id_node *parent, fl_id_entry *entry);

fl_void fl_context_load_module(fl_context *cnt, fl_id_chain *id_chain, fl_bool global_scope);

fl_void fl_context_free(fl_context *cnt);
