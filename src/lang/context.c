#include "context.h"

#include "tools/lib.h"
#include "shared/error.h"

// ---- DEFINES ----

#define _FL_MODULE_REGISTER_FN_NAME "fl_module_register"
#define _FL_ID_CHAIN_STRING_MAX_SIZE 256

typedef fl_id_node *(*fl_module_register_fn)(fl_context *cnt, fl_id_chain *chain);

// ---- DEFINITIONS ----

fl_void fl_context_init(fl_context *cnt)
{
    fl_id_node_vec_init(&cnt->global_scopes, 8);
    fl_lib_handle_vec_init(&cnt->lib_handles, 0);
    fl_byte_code_init(&cnt->byte_code, 0);
    fl_id_node_vec_init(&cnt->id_list, 0);

    cnt->id_tree = FL_NULL;
}

fl_void fl_context_free(fl_context *cnt)
{
    fl_id_node_vec_free(&cnt->global_scopes);
    fl_lib_handle_vec_free(&cnt->lib_handles);
    fl_byte_code_free(&cnt->byte_code);
    fl_id_node_vec_free(&cnt->id_list);
    fl_id_node_free(cnt->id_tree);

    cnt->id_tree = FL_NULL;
}

fl_id_node *fl_context_register_id(fl_context *cnt, fl_id_node *parent, fl_id_entry *entry)
{
    assert(entry);

    fl_id_node *node;

    if (!parent)
    {
        if (cnt->id_tree)
            parent = cnt->id_tree;
        else
        {
            fl_id_node_init(&cnt->id_tree, entry);
            node = cnt->id_tree;
            goto __res;
        }
    }

    // TODO: handle push fail
    node = fl_id_node_push_child(parent, entry);

__res:
    fl_id_node_vec_push(&cnt->id_list, node);
    return node;
}

fl_int fl_context_resolve_id(fl_context *cnt, fl_id_chain *id_chain, const fl_id_node **id_node)
{
    *id_node = fl_id_node_seek(cnt->id_tree, id_chain);
    return fl_id_node_vec_find(&cnt->id_list, *id_node);
}

fl_void fl_context_load_module(fl_context *cnt, fl_id_chain *id_chain, fl_bool global_scope)
{
    assert(id_chain->size);

    // first id should be module name
    fl_lib_handle handle = fl_lib_load(id_chain->data[0]);
    // TODO: change error params
    if (!handle)
        fl_err_throw(FL_ERR_IMPORT, "Couldn't find the library", id_chain->data[0], (fl_location_info){.description = "global context"});

    fl_lib_handle_vec_push(&cnt->lib_handles, handle);

    fl_module_register_fn reg_fn = (fl_module_register_fn)fl_lib_get_address(handle, _FL_MODULE_REGISTER_FN_NAME);
    if (!reg_fn)
        fl_err_throw(FL_ERR_IMPORT, "Couldn't find module registration function", id_chain->data[0], (fl_location_info){.description = "global context"});

    fl_id_node *leaf = reg_fn(cnt, id_chain);
    if (!leaf)
    {
        fl_char buffer[_FL_ID_CHAIN_STRING_MAX_SIZE];
        fl_id_chain_to_string(id_chain, buffer, _FL_ID_CHAIN_STRING_MAX_SIZE);
        fl_err_throw(FL_ERR_IMPORT, "Couldn't import id chain", buffer, (fl_location_info){.description = "global context"});
    }

    // add id to gobal scopes
    if (global_scope && fl_id_node_vec_find(&cnt->global_scopes, leaf) == -1)
        fl_id_node_vec_push(&cnt->global_scopes, leaf);
}