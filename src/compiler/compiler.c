#include "compiler.h"

#include "shared/memory.h"
#include "tools/file.h"
#include "shared/error.h"
#include "shared/token.h"
#include "shared/token_array.h"
#include "tokenizer/tokenizer.h"
#include "compiler/ast.h"

#include "core/action.h"
#include "tools/printer.h"

// --- DEFINES ---

#define throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_token_location){.description = source_path})

// --- STATIC FUNCTION DEFINITIONS ---

static void _fl_comp_compile_node(fl_ast_node *root, fl_context *cnt);

// --- IMPLEMENTATIONS ---

// compiles source code to intermediete byte code
fl_void fl_comp_compile(fl_char *source_path, fl_context *cnt)
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

    fl_prt_print_ast_tree(&tree);
    return;

    _fl_comp_compile_node(tree.root, cnt);

    fl_token_arr_free(&token_array);
    fl_ast_tree_free(&tree);
}

fl_void fl_context_init(fl_context *cnt)
{
    fl_byte_code_init(&cnt->byte_code);
    fl_id_table_init(&cnt->id_table);
}

fl_void fl_context_free(fl_context *cnt)
{
    fl_byte_code_free(&cnt->byte_code);
    fl_id_table_free(&cnt->id_table);
}

static void _fl_comp_compile_node(fl_ast_node *root, fl_context *cnt)
{
    fl_ast_node *current = root;
    while (current)
    {
        switch (current->type)
        {
        case FL_AST_NODE_TYPE_BLOCK:
            fl_byte_code_push(&cnt->byte_code, FL_COMP_INSTR_STARTBLOCK);
            _fl_comp_compile_node(current->child, cnt);
            fl_byte_code_push(&cnt->byte_code, FL_COMP_INSTR_ENDBLOCK);
            break;
        case FL_AST_NODE_TYPE_CALL:
            fl_byte_code_push(&cnt->byte_code, FL_COMP_INSTR_CALL);
            break;
        case FL_AST_NODE_TYPE_OPERATOR:
        default:
            break;
        }

        current = current->next;
    }
}