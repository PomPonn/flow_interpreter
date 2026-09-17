/**
 * ---------- shared.h ------------------
 *
 * Declares shared compiler logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "shared/error.h"

// ------------- MACROS -----------------

/**
 * @brief Shorthand for throwing a compilation error.
 */
#define fl_throw_compilation_error(ast_node_ptr, message)                          \
    {                                                                              \
        fl_char buffer[FL_ID_CHAIN_MAX_SIZE];                                      \
        fl_id_chain_to_string(ast_node_ptr->data, buffer, FL_ID_CHAIN_MAX_SIZE);   \
        fl_err_throw(FL_ERR_COMPILATION, message, buffer, ast_node_ptr->location); \
    }

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_cstr_vec);
FL_FORWARD_STRUCT(fl_ast_node);
FL_FORWARD_STRUCT(fl_operation);
FL_FORWARD_STRUCT(fl_cmp_context);

// ---------- DECLARATIONS --------------

fl_void fl_cmp_block_cnt_to_bytes(fl_byte_vec *bytes, const fl_cmp_context *cnt, const fl_cstr_vec *ignore_labels);

/**
 * @brief Compiles function call AST nodes.
 * @param cnt
 * @param id_node
 * @param op
 * @return
 */
fl_void fl_cmp_call_to_bytes(fl_ast_node *id_node, fl_operation op, fl_cmp_context *cnt);