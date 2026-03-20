/**
 * ---- scope.h ----
 *
 * Declares flow scope logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "struct/id/id_node_vec.h"
#include "struct/scope_vec.h"

// ---- TYPE DEFINITIONS ----

struct _fl_scope
{
    const struct _fl_scope *outer;
    fl_id_node_vec ids;
    fl_scope_vec inner;
};
typedef struct _fl_scope fl_scope;

// ---- DECLARATIONS ----

FL_HEADER_FN fl_void fl_scope_init(fl_scope *scope, const fl_scope *outer)
{
    fl_id_node_vec_init(&scope->ids, 0);
    fl_scope_vec_init(&scope->inner, 0);
    scope->outer = outer;
}

FL_HEADER_FN fl_void fl_scope_free(fl_scope *scope)
{
    fl_id_node_free(&scope->ids);
    fl_scope_vec_free(&scope->inner);
}