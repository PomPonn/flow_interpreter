/**
 * ---- scope.h ----
 *
 * Declares flow scope logic.
 *
 */

#pragma once

// ---- FORWARD TYPEDEFS ----

struct _fl_scope_vec;

// ---- DEPENDENCIES ----

#include "struct/id/id_entry_vec.h"
#include "struct/int_vec.h"

// ---- TYPE DEFINITIONS ----

struct _fl_scope
{
    const struct _fl_scope *outer;
    fl_id_entry_vec ids;
};
typedef struct _fl_scope fl_scope;

// ---- DECLARATIONS ----

FL_HEADER_FN fl_void fl_scope_init(fl_scope *scope, const fl_scope *outer)
{
    fl_id_entry_vec_init(&scope->ids, 0);
    scope->outer = outer;
}

FL_HEADER_FN fl_void fl_scope_free(fl_scope *scope)
{
    fl_id_entry_vec_free(&scope->ids);
    scope->outer = FL_NULL;
}

fl_int_vec fl_scope_resolve_id(const fl_scope *scope, fl_id_chain *id_chain, fl_uint *outer_offset);

fl_id_entry *fl_scope_traverse(const fl_scope *scope, fl_int_vec inds, fl_uint back_offset);