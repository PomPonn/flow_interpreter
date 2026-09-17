/**
 * ---------- compiler.h --------------
 *
 * Declares compiler logic.
 *
 * ------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ------------

#include "shared/common.h"
#include "shared/error.h"

// ------------- MACROS -----------------

/**
 * @brief Shorthand for throwing a compilation error.
 */
#define __fl_throw_compilation_error(ast_node_ptr, message)                        \
    {                                                                              \
        fl_char buffer[FL_ID_CHAIN_MAX_SIZE];                                      \
        fl_id_chain_to_string(ast_node_ptr->data, buffer, FL_ID_CHAIN_MAX_SIZE);   \
        fl_err_throw(FL_ERR_COMPILATION, message, buffer, ast_node_ptr->location); \
    }

// ---------- TYPE DEFINITIONS --------

FL_FORWARD_STRUCT(fl_ast_node);
FL_FORWARD_STRUCT(fl_byte_vec);
FL_FORWARD_STRUCT(fl_cstr_vec);
FL_FORWARD_STRUCT(fl_operation);
FL_FORWARD_STRUCT(fl_cmp_context);

// ---------- DECLARATIONS ------------

/**
 * @brief Compile AST tree to byte code.
 * @param tree AST tree to compile.
 * @return Compiled byte code.
 */
fl_byte_vec fl_cmp_compile_ast(const fl_ast_node *tree);

/**
 * @brief Compile single AST node.
 * @param node Node to compile.
 * @param cnt Context to use while compilation.
 */
fl_void fl_cmp_compile_node(const fl_ast_node *node, fl_cmp_context *cnt);

/**
 * @brief Convert compilation context as a code block to bytes.
 * @param bytes Byte vector to write to.
 * @param cnt Context to convert.
 * @param ignore_labels Optional id labels to ignore when
 *                      declaring ids at the beginning of the block.
 */
fl_void fl_cmp_block_cnt_to_bytes(
    fl_byte_vec *bytes,
    const fl_cmp_context *cnt,
    const fl_cstr_vec *ignore_labels);

/**
 * @brief Compiles operation call AST nodes.
 * @param id_node AST node of id of the operation.
 * @param op Operation to compile.
 * @param cnt Context to use.
 */
fl_void fl_cmp_call_to_bytes(
    const fl_ast_node *id_node,
    const fl_operation op,
    fl_cmp_context *cnt);