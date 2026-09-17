/**
 * ---------- scope.h ---------------
 *
 * Declares flow scope logic.
 *
 * ----------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "vecs/id_entry_vec.h"
#include "vecs/int_vec.h"

// ---------- DEFINES ---------------

#define fl_scope_get_inners(scope_ptr) ((fl_scope_vec *)(scope_ptr)->inners)

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_id_chain);

typedef fl_void *fl_inner_scopes_impl;

struct _fl_scope
{
    struct _fl_scope *outer;
    fl_id_entry_vec ids;
    fl_inner_scopes_impl inners;
};
typedef struct _fl_scope fl_scope;

// ---------- DECLARATIONS ----------

FL_SEALED_FN fl_void fl_scope_add_inner(fl_scope *scope, fl_scope *inner);
FL_SEALED_FN fl_id_entry *fl_scope_resolve_id(
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

fl_void fl_scope_free(fl_scope **scope);
fl_void fl_scope_init(fl_scope **scope, fl_scope *outer);
fl_void fl_scope_to_bytes(const fl_scope *scope, fl_byte_vec *byte_vec);

// depends on fl_scope_free
#include "vecs/scope_vec.h"

// ---------- DEFINITIONS ----------

FL_SEALED_FN fl_id_entry *fl_scope_resolve_id(
    const fl_scope *scope,
    const fl_id_chain *id_chain)
{
    fl_uint offset;
    fl_int_vec inds = fl_scope_find_id_path(scope, id_chain, &offset);

    if (fl_int_vec_is_empty(&inds))
        return FL_NULL;

    fl_id_entry *id = fl_scope_traverse(scope, inds, offset);

    fl_int_vec_free(&inds);
    return id;
}

FL_SEALED_FN fl_void fl_scope_add_inner(fl_scope *scope, fl_scope *inner)
{
    fl_scope_vec_push(fl_scope_get_inners(scope), inner);
    inner->outer = scope;
}