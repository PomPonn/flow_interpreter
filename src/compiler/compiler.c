#include "compiler.h"

#include "shared/memory.h"
#include "tools/file.h"
#include "shared/error.h"
#include "shared/token.h"
#include "shared/token_array.h"
#include "tokenizer/tokenizer.h"
#include "compiler/ast.h"

// byte code array
#define FL_ARRAY_NAME bcode
#define FL_ARRAY_TYPE fl_ubyte
#define FL_ARRAY_CAP_STEP 16
#include "generics/array.h"

// id table array
#define FL_ARRAY_NAME id_table
#define FL_ARRAY_TYPE char *
#define FL_ARRAY_CAP_STEP 16
#include "generics/array.h"

#include "core/action.h"

// --- DEFINES ---

#define _FL_BCODE_CAP_STEP 1024

#define throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_token_location){.description = source_path})

// --- STATIC FUNCTION DEFINITIONS ---

static void _fl_comp_compile_node(fl_ast_node *root, fl_ubyte *bcode);

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

    fl_ast_tree tree;
    fl_ast_tree_gen(&tree, &token_array);

    fl_bcode bcode;
    fl_bcode_init(&bcode, _FL_BCODE_CAP_STEP);

    _fl_comp_compile_node(tree.root, &bcode);

    fl_token_arr_free(&token_array);
    fl_ast_tree_free(&tree);
}

static void _fl_comp_compile_node(fl_ast_node *root, fl_ubyte *bcode)
{
    fl_ast_node *current = root;
    while (current)
    {
        switch (current->type)
        {
        case FL_AST_NODE_TYPE_BLOCK:
            fl_bcode_push(bcode, FL_COMP_INSTR_STARTBLOCK);
            _fl_comp_compile_node(current->child, bcode);
            fl_bcode_push(bcode, FL_COMP_INSTR_ENDBLOCK);
            break;
        case FL_AST_NODE_TYPE_CALL:
            fl_bcode_push(bcode, FL_COMP_INSTR_CALL);
            break;
        case FL_AST_NODE_TYPE_OPERATOR:
        default:
            break;
        }

        current = current->next;
    }
}