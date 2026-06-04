/**
 * ---------- flow.c ----------------------
 *
 * Implements main flow language logic.
 *
 * ----------------------------------------
 */

#include "flow.h"

// ---------- DEPENDENCIES ----------

#include "compiler/compiler.h"
#include "tokenizer/token_arr.h"
#include "interpreter/interpreter.h"
#include "ast/ast_tree.h"
#include "tools/file.h"
#include "shared/error.h"
#include "tokenizer/tokenizer.h"
#include "ast/ast_tree.h"
#include "tools/file.h"
#include "flow/globals.h"
#include "context/base_context.h"

#ifndef NDEBUG
#include "tools/printer.h"
#include <stdio.h>
#endif

// ---------- MACROS ----------------

/**
 * @brief Shorthand for throwing a file error.
 */
#define throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_token_location){.description = source_path})

// ---------- DECLARATIONS ----------

// ...

// ---------- DEFINITIONS -----------

fl_void fl_flow_run(const fl_char *source_path)
{
    if (!fl_globals_initialized())
        assert(FL_FALSE);

    fl_char *source_code = fl_file_read(source_path);

    if (!source_code)
        throw_file_error("[ERROR] Couldn't read the file (does it exist?)");

    printf("------ Source code: ------\n%s\n\n", source_code);

    fl_token_arr token_arr;
    fl_bool any_token = fl_tok_tokenize(source_code, &token_arr, source_path);

#ifndef NDEBUG
    printf("------ Tokens: ------\n");
    fl_prt_print_token_entries(&token_arr);
#endif

    fl_file_content_free(&source_code);

    if (!any_token)
        // there are no meaningful tokens in the file
        return;

    fl_ast_node *tree = fl_ast_tree_gen(&token_arr);

#ifndef NDEBUG
    printf("\n------ AST nodes: ------\n");
    fl_prt_print_ast_tree(tree);
#endif

    return;

    fl_base_context cnt;
    fl_base_context_init(&cnt);
    fl_byte_vec code = fl_cmp_compile_ast(tree, &cnt);

#ifndef NDEBUG
    // save byte code to file
    fl_file_write("bytecode.cfl", code);
    printf("\n------ Byte code: ------\n");
    fl_prt_print_byte_code(code);
#endif

    fl_token_arr_free(&token_arr);
    fl_ast_node_free(&tree);

    printf("\n------ Interpretation: ------\n");

    // reuse base context ???

    fl_intpr_interpret(code);

    fl_byte_vec_free(&code);
    fl_base_context_free(&cnt);
}