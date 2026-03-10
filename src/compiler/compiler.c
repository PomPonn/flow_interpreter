#include "compiler.h"

#include "shared/memory.h"
#include "tools/file.h"
#include "shared/error.h"
#include "tokenizer/token.h"
#include "tokenizer/token_array.h"
#include "tokenizer/tokenizer.h"
#include "structs/id/id_tree.h"
#include "ast/ast.h"

#include "tools/printer.h"

// ---- DEFINES ----

#define _ID_STRING_BUFFER_SIZE 512

#define throw_file_error(message) \
    fl_err_throw(FL_ERR_FILE, message, FL_NULL, (fl_token_location){.description = source_path})

// ---- DECLARATIONS ----

static void _fl_comp_compile_node(fl_ast_node *root, fl_context *cnt);

// ---- DEFINITIONS ----

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
    fl_char buffer[_ID_STRING_BUFFER_SIZE];
    fl_ast_node *node = root;
    fl_byte_code bc = cnt->byte_code;

    while (node)
    {
        switch (node->type)
        {
        case FL_AST_NODE_TYPE_BLOCK:
            fl_byte_code_push(&bc, FL_INSTR_STARTBLOCK);
            _fl_comp_compile_node(node->child, cnt);
            fl_byte_code_push(&bc, FL_INSTR_ENDBLOCK);
            break;
        case FL_AST_NODE_TYPE_CALL:
            fl_byte_code_push(&bc, FL_INSTR_CALL);
            // first child is a function id chain node
            fl_ast_node *id_node = node->child;
            fl_id_chain *id_chain = (fl_id_chain *)id_node->data;

            // seek for this function id
            const fl_id_entry *func_id = fl_id_tree_seek_value(cnt->id_tree, id_chain);

            if (!func_id)
            {
                // throw an error if it's absent
                fl_id_chain_to_string(id_chain, buffer, _ID_STRING_BUFFER_SIZE);
                fl_err_throw(
                    FL_ERR_COMPILATION,
                    "Use of unknown id",
                    buffer,
                    id_node->location);
            }

            if (func_id->type != FL_ID_TYPE_FUNC)
            {
                // throw an error if it's absent
                fl_id_chain_to_string(id_chain, buffer, _ID_STRING_BUFFER_SIZE);
                fl_err_throw(
                    FL_ERR_COMPILATION,
                    "id is not callable (not a function)",
                    buffer,
                    id_node->location);
            }

            // push id index
            fl_int id_index = fl_fn_list_find(&cnt->func_list, (fl_fn_entry *)func_id);
            fl_byte_code_push_mem(&bc, &id_index, sizeof(id_index));

            // process function arguments (if any)
            fl_ast_node *arg = id_node->next;
            while (arg)
            {
                // fl_byte_code_push(  
                // ...
                arg = arg->next;
            }
            
            break;
        case FL_AST_NODE_TYPE_OPERATOR:
        default:
            break;
        }

        node = node->next;
    }
}
