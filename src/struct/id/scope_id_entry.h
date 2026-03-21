#pragma once

#include "shared/common.h"
#include "struct/id/id_entry_vec.h"
#include "lang/scope.h"

struct _fl_scope_id_entry
{
    FL_EXTEND_ID_ENTRY;
    fl_scope scope;
};
typedef struct _fl_scope_id_entry fl_scope_id_entry;

FL_HEADER_FN fl_void fl_scope_id_entry_init(
    fl_scope_id_entry **entry,
    const fl_char *name,
    const fl_scope *outer_scope)
{
    fl_scope_id_entry *val = fl_mem_alloc(sizeof(fl_scope_id_entry));
    fl_id_entry_fill(val, name, FL_ID_ENTRY_TYPE_SCOPE);
    fl_scope_init(&val->scope, outer_scope);
    *entry = val;
}

FL_HEADER_FN fl_void fl_scope_id_entry_free(fl_scope_id_entry **entry)
{
    fl_scope_free(&(*entry)->scope);
    fl_mem_free(*entry);
    *entry = FL_NULL;
}