#include "compiler.h"

#include "tools/file.h"
#include "shared/error.h"
#include "shared/token.h"
#include "shared/token_array.h"
#include "tokenizer/tokenizer.h"
#include "compiler/ast.h"

// --- DEFINES ---

#define throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_token_location){.description = source_path})

// --- IMPLEMENTATIONS ---

// compiles source code to intermediete byte code
fl_void fl_comp_compile(fl_char *source_path)
{
    fl_char *source_code = fl_file_read(source_path);

    if (!source_code)
        throw_file_error("Couldn't read the file (does it exist?)");

    fl_token_arr token_array;
    fl_bool any_token = fl_tok_tokenize(source_code, &token_array, source_path);

    fl_file_free(source_code);

    if (!any_token)
        // there are no tokens in the file
        return;

    fl_ast_node *root = fl_ast_gen(&token_array);
}
