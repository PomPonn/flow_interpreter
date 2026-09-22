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

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_id_chain);
FL_FORWARD_STRUCT(fl_scope_vec);

typedef fl_void *fl_inner_scopes_impl;

struct _fl_scope
{
    struct _fl_scope *outer;
    fl_id_entry_vec ids;
    fl_inner_scopes_impl inners;
    fl_bool freeze_blocks;
};
typedef struct _fl_scope fl_scope;

// ---------- DECLARATIONS ----------

FL_INLINE_FN fl_scope_vec *fl_scope_get_inners(const fl_scope *scope);

FL_INLINE_FN fl_void fl_scope_add_inner(fl_scope *scope, fl_scope *inner);

FL_INLINE_FN fl_void fl_scope_freeze_blocks(fl_scope *scope, fl_bool val);

fl_id_entry *fl_scope_find_id(const fl_scope *scope, const fl_id_chain *id_chain);

fl_void fl_scope_free(fl_scope **scope);
fl_void fl_scope_init(fl_scope **scope, fl_scope *outer);
fl_void fl_scope_to_bytes(const fl_scope *scope, fl_byte_vec *byte_vec);

// depends on fl_scope_free
#include "vecs/scope_vec.h"

// ---------- DEFINITIONS ----------

FL_INLINE_FN fl_scope_vec *fl_scope_get_inners(const fl_scope *scope)
{
    return scope->inners;
}

FL_INLINE_FN fl_void fl_scope_add_inner(fl_scope *scope, fl_scope *inner)
{
    fl_scope_vec_push(fl_scope_get_inners(scope), inner);
    inner->outer = scope;
}

FL_INLINE_FN fl_void fl_scope_freeze_blocks(fl_scope *scope, fl_bool val)
{
    scope->freeze_blocks = val;
}    
