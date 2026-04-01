#include "context.h"

#include "tools/lib.h"
#include "shared/error.h"
#include "tools/mod_importer.h"

// ---------- MACROS ----------

#define _FL_ID_CHAIN_STRING_MAX_SIZE 256

// ---------- DEFINITIONS ----------

fl_void fl_context_init(fl_context *cnt)
{
    fl_lib_handle_vec_init(&cnt->lib_handles);
    fl_cstr_vec_init(&cnt->lib_names);
    fl_scope_init(&cnt->main_scope, FL_NULL);
    fl_const_scope_vec_init(&cnt->usings);
    fl_value_vec_init(&cnt->literals);
    fl_predef_vec_init(&cnt->predefines);
}

fl_void fl_context_free(fl_context *cnt)
{
    fl_lib_handle_vec_free(&cnt->lib_handles);
    fl_cstr_vec_free(&cnt->lib_names);
    fl_scope_free(&cnt->main_scope);
    fl_const_scope_vec_free(&cnt->usings);
    fl_value_vec_free(&cnt->literals);
    fl_predef_vec_free(&cnt->predefines);
}

// fl_id_node *fl_context_register_id(fl_context *cnt, fl_id_node *parent, fl_id_entry *entry)
// {
//     assert(entry);

//     fl_id_node *node;

//     if (!parent)
//     {
//         if (cnt->id_tree)
//             parent = cnt->id_tree;
//         else
//         {
//             fl_id_node_init(&cnt->id_tree, entry);
//             node = cnt->id_tree;
//             goto __res;
//         }
//     }

//     // TODO: handle push fail
//     node = fl_id_node_push_child(parent, entry);

// __res:
//     fl_id_node_vec_push(&cnt->id_list, node);
//     return node;
// }

fl_void fl_context_register_id(fl_context *cnt, fl_scope *scope, fl_id_entry *entry)
{
    assert(entry);

    if (scope)
        fl_id_entry_vec_push(&scope->ids, entry);
    else
        fl_id_entry_vec_push(&cnt->main_scope.ids, entry);
}

fl_int fl_context_resolve_id(
    fl_context *cnt,
    fl_id_node *scope,
    fl_id_chain *id_chain,
    fl_id_node **id_node)
{
    *id_node = FL_NULL;

    if (scope)
        // seek in private scope
        *id_node = fl_id_node_seek(scope, id_chain);

    if (!*id_node)
    {
        // seek in root scope
        // *id_node = fl_id_node_seek(cnt->id_tree, id_chain);

        // if (!*id_node)
        // seek in global scopes
        // for (fl_size i = 0; i < cnt->global_scopes.size; i++)
        // {
        //     *id_node = fl_id_node_seek(cnt->global_scopes.data[i]->child, id_chain);
        //     if (*id_node)
        //         break;
        // }
    }

    // return fl_id_node_vec_find(&cnt->id_list, *id_node);
    assert(FL_FALSE);
    return FL_FIND_FAIL;
}

fl_bool fl_context_add_global_scope(fl_context *cnt, fl_id_node *id_node)
{
    // if (fl_id_node_vec_find(&cnt->global_scopes, id_node) == FL_FIND_FAIL)
    // {
    //     fl_id_node_vec_push(&cnt->global_scopes, id_node);
    //     return FL_TRUE;
    // }

    return FL_FALSE;
}

fl_void fl_context_load_module_by_str(fl_context *cnt, const fl_char *id_str)
{
    fl_id_entry *id = fl_mimp_import_id(cnt, id_str);
    assert(id);

    fl_id_entry_vec_push(&cnt->main_scope.ids, id);
}