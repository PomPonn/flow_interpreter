#include "context.h"

#include "tools/lib.h"
#include "shared/error.h"
#include "tools/mod_importer.h"

// ---------- MACROS ----------

#define _FL_ID_CHAIN_STRING_MAX_SIZE 256

// ---------- DEFINITIONS ----------

fl_void fl_context_register_id(fl_context *cnt, fl_scope *scope, fl_id_entry *entry)
{
    assert(entry);

    assert(scope);

    fl_id_entry_vec_push(&scope->ids, entry);
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