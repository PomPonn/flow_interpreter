//// ?????????????????????????????????????

/**
 * ---------- ast_gen_context.h --------------
 *
 * Declares AST generation context logic.
 *
 * -------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "ast/ast_node.h"
#include "tokenizer/token_arr.h"
#include "tokenizer/token_location.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_ast_gen_context
{
    fl_ast_node *parent;
    fl_token_arr *toks;
};
/**
 * @brief Describes interpreter context.
 */
typedef struct _fl_ast_gen_context fl_ast_gen_context;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void fl_ast_gen_context_init(fl_ast_gen_context *cnt, fl_token_arr *token_arr);
FL_HEADER_FN fl_void fl_ast_gen_context_free(fl_ast_gen_context *cnt);

// ---------- DEFINITIONS --------------

FL_HEADER_FN fl_void fl_ast_gen_context_free(fl_ast_gen_context *cnt)
{
    fl_ast_node_free(&cnt->parent);
    fl_token_arr_free(cnt->toks);
}

FL_HEADER_FN fl_void fl_ast_gen_context_init(fl_ast_gen_context *cnt, fl_token_arr *token_arr)
{
    fl_ast_node_init(&cnt->parent, FL_AST_NODE_TYPE_BLOCK, FL_NULL, FL_NULL, FL_TOKEN_LOCATION_EMPTY);
    cnt->toks = token_arr;

    fl_token_arr_reset_ptr(token_arr);
}