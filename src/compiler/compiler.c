#include "compiler.h"

#include "shared/memory.h"
#include "tools/file.h"
#include "shared/error.h"
#include "shared/token.h"
#include "shared/token_array.h"
#include "tokenizer/tokenizer.h"
#include "compiler/ast.h"
#include "compiler/id_view.h"

#include "core/action.h"
#include "tools/printer.h"

// --- DEFINES ---

#define ID_STRING_BUFFER_SIZE 512

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

    _fl_comp_compile_node(tree.root, cnt);

    fl_token_arr_free(&token_array);
    fl_ast_tree_free(&tree);
}

static void _fl_comp_compile_node(fl_ast_node *root, fl_context *cnt)
{
    fl_ast_node *node = root;
    fl_byte_code bc = cnt->byte_code;

    while (node)
    {
        switch (node->type)
        {
        case FL_AST_NODE_TYPE_BLOCK:
            fl_byte_code_push(&bc, FL_COMP_INSTR_STARTBLOCK);
            _fl_comp_compile_node(node->child, cnt);
            fl_byte_code_push(&bc, FL_COMP_INSTR_ENDBLOCK);
            break;
        case FL_AST_NODE_TYPE_CALL:
            fl_byte_code_push(&bc, FL_COMP_INSTR_CALL);
            // first child is a function id chain
            fl_ast_node *child = node->child;
            // check if function id exists by traversing the id view built before
            if (!fl_id_view_node_valid_id_chain(cnt->id_view, child))
            {
                // throw an error if it's absent
                fl_char buffer[ID_STRING_BUFFER_SIZE];
                fl_ast_stringify_id_chain(child, buffer, ID_STRING_BUFFER_SIZE);

                fl_err_throw(
                    FL_ERR_COMPILATION,
                    "Use of unknown id",
                    buffer,
                    child->location);
            }

            // include id_chain in view of ids
            fl_id_view_node_include_id_chain(cnt->id_view, child);
            // leave an index in byte code to recognise this id later
            fl_size id_index = fl_context_next_id_table_index(cnt);
            fl_byte_code_push_mem(&bc, &id_index, sizeof(id_index));

            // GO ON

            break;
        case FL_AST_NODE_TYPE_OPERATOR:
        default:
            break;
        }

        node = node->next;
    }
}