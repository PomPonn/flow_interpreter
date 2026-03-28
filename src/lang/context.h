/**
 * ---- context.h ----
 *
 * Declares language context logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "vecs/id_node_vec.h"
#include "vecs/scope_vec.h"
#include "vecs/byte_vec.h"
#include "vecs/value_vec.h"
#include "vecs/lib_handle_vec.h"

// ---- TYPE DEFINITIONS ----

struct _fl_context
{
    fl_lib_handle_vec lib_handles;
    fl_scope main_scope;
    fl_scope_vec usings;
    fl_value_vec literals;
    // fl_byte_vec byte_vec;
};
/**
 * @brief Describes language context, storing global information (?).
 */
typedef struct _fl_context fl_context;

/**
 * @brief Initializes context structure.
 * @param cnt Context ot initialize.
 */
fl_void fl_context_init(fl_context *cnt);

/**
 * @brief Resolves id chain with given context.
 * @param cnt Context to use.
 * @param scope ???
 * @param id_chain Id chain to resolve.
 * @param [out] id_node Resolved node or `FL_NULL` if failed to resolve.
 * @return Index of id node in context id list.
 */
fl_int fl_context_resolve_id(
    fl_context *cnt,
    fl_id_node *scope,
    fl_id_chain *id_chain,
    fl_id_node **id_node);

FL_HEADER_FN fl_void fl_context_load_module_by_str(fl_context *cnt, const fl_char *id_str);

// TODO: comment when finished

/**
 * @brief
 * @param cnt
 * @param parent
 * @param entry
 * @return
 */
fl_void fl_context_register_id(fl_context *cnt, fl_scope *scope, fl_id_entry *entry);

fl_void fl_context_load_module_by_id(fl_context *cnt, fl_id_chain *id_chain);

fl_bool fl_context_add_global_scope(fl_context *cnt, fl_id_node *id_node);

FL_HEADER_FN fl_uint fl_context_make_literal(fl_context *cnt, fl_value lit_value)
{
    fl_uint ind = fl_value_vec_find(&cnt->literals, lit_value);

    if (ind == FL_FIND_FAIL)
        fl_value_vec_push(&cnt->literals, lit_value);

    return ind;
}

fl_void fl_context_free(fl_context *cnt);

FL_HEADER_FN fl_void fl_context_load_module_by_str(fl_context *cnt, const fl_char *id_str)
{
    fl_id_chain _chain;
    fl_id_chain_init_parse(&_chain, id_str);

    fl_context_load_module_by_id(cnt, &_chain);

    fl_id_chain_free(&_chain);
}
