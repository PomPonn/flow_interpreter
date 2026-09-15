/**
 * ---------- flow.c ----------------------
 *
 * Implements main flow language logic.
 *
 * ----------------------------------------
 */

#include "flow.h"

// ---------- DEPENDENCIES -----------

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
#include "vecs/byte_vec.h"

#ifndef NDEBUG
#include "tools/printer.h"
#include <stdio.h>
#endif

// ---------- MACROS ----------------

/**
 * @brief Shorthand for throwing a file error.
 */
#define fl_throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_token_location){.description = source_path})

// ---------- DEFINITIONS -----------

fl_void fl_flow_run(const fl_char *source_path)
{
    if (!fl_globals_initialized())
        assert(FL_FALSE);

    // fl_prof_profile_once(fl_file_read(source_path));
    // return;

    fl_char *source_code = fl_file_read(source_path);
    if (!source_code)
        fl_throw_file_error("[ERROR] Couldn't read the file (does it exist?)");

#ifndef NDEBUG
    printf("------ Source code: ------\n%s\n\n", source_code);
#endif

    // fl_prof_profile_once(fl_tok_tokenize(source_code, source_path));
    // return;

    fl_token_arr token_arr = fl_tok_tokenize(source_code, source_path);

#ifndef NDEBUG
    printf("------ Tokens: ------\n");
    fl_prt_print_token_entries(&token_arr);
#endif

    fl_file_content_free(&source_code);

    if (fl_token_arr_is_empty(&token_arr))
        // there are no meaningful tokens in the file
        return;

    // fl_prof_profile_once(fl_ast_tree_gen(&token_arr));
    // return;

    fl_ast_node *tree = fl_ast_tree_gen(&token_arr);

    fl_token_arr_free(&token_arr);

#ifndef NDEBUG
    printf("\n------ AST nodes: ------\n");
    fl_prt_print_ast_tree(tree);
#endif

    // fl_prof_profile_once(fl_cmp_compile_ast(tree));

    fl_byte_vec code = fl_cmp_compile_ast(tree);

    fl_ast_node_free(&tree);

#ifndef NDEBUG
    // save byte code to file
    fl_file_write("bytecode.cfl", code);
    printf("\n------ Byte code: ------\n");
    fl_prt_print_byte_code_file(code);
#endif

#ifndef NDEBUG
    printf("\n------ Interpretation: ------\n");
#endif

    // fl_prof_profile_once(fl_intpr_interpret(code));
    // return;

    fl_intpr_interpret(code);

    fl_byte_vec_free(&code);
}