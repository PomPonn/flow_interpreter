/**
 * ---- context.h ----
 *
 * Declares language context logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "struct/id/id_node_vec.h"
#include "struct/id/scope_id_data.h"
#include "struct/byte_vec.h"
#include "struct/lib_handle_vec.h"

// ---- TYPE DEFINITIONS ----

struct _fl_context
{
    fl_scope main_scope;
    fl_lib_handle_vec lib_handles;
    fl_id_node *id_tree;
    fl_id_node_vec id_list;
    fl_byte_vec byte_vec;
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

// TODO: comment when finished

/**
 * @brief
 * @param cnt
 * @param parent
 * @param entry
 * @return
 */
// fl_id_node *fl_context_register_id(fl_context *cnt, fl_id_node *parent, fl_id_entry *entry);

fl_void fl_context_register_id(fl_context *cnt, fl_scope *scope, fl_id_entry *entry);

fl_void fl_context_load_module_by_id(fl_context *cnt, fl_id_chain *id_chain);

fl_void fl_context_load_module_by_str(fl_context *cnt, const fl_char *id_str);

fl_bool fl_context_add_global_scope(fl_context *cnt, fl_id_node *id_node);

fl_void fl_context_free(fl_context *cnt);