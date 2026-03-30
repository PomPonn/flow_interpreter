/**
 * ---- scope.h ----
 *
 * Declares flow scope logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "vecs/id_entry_vec.h"
#include "vecs/int_vec.h"
#include "vecs/byte_vec.h"
#include "lang/id_chain.h"

// ---- TYPE DEFINITIONS ----

struct _fl_scope
{
    const struct _fl_scope *outer;
    fl_id_entry_vec ids;
};
typedef struct _fl_scope fl_scope;

// ---- DECLARATIONS ----

FL_HEADER_FN fl_void fl_scope_init(fl_scope *scope, const fl_scope *outer);
FL_HEADER_FN fl_void fl_scope_free(fl_scope *scope);
FL_HEADER_FN fl_id_entry *fl_scope_resolve_id(
    const fl_scope *scope,
    const fl_id_chain *id_chain);

fl_int_vec fl_scope_find_id_path(
    const fl_scope *scope,
    const fl_id_chain *id_chain,
    fl_uint *outer_offset);

fl_id_entry *fl_scope_traverse(
    const fl_scope *scope,
    const fl_int_vec inds,
    fl_uint back_offset);

fl_void fl_scope_to_bytes(const fl_scope *scope, fl_byte_vec *byte_vec);

// ---- HEADER-ONLY IMPLEMENTATIONS ----

FL_HEADER_FN fl_void fl_scope_init(fl_scope *scope, const fl_scope *outer)
{
    fl_id_entry_vec_init(&scope->ids);
    scope->outer = outer;
}

FL_HEADER_FN fl_void fl_scope_free(fl_scope *scope)
{
    fl_id_entry_vec_free(&scope->ids);
    scope->outer = FL_NULL;
}

FL_HEADER_FN fl_id_entry *fl_scope_resolve_id(
    const fl_scope *scope,
    const fl_id_chain *id_chain)
{
    fl_uint offset;
    fl_int_vec inds = fl_scope_find_id_path(scope, id_chain, &offset);

    fl_id_entry *id = fl_scope_traverse(scope, inds, offset);

    fl_int_vec_free(&inds);
    return id;
}