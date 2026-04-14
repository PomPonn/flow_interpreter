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