#include <stdio.h>

#include "tools/file.h"
#include "tools/printer.h"
#include "optimalizer/optimalizer.h"
#include "tokenizer/tokenizer.h"
#include "compiler/ast.h"

#include "tools/profiler.h"

int main()
{
    const char *filename = _PROJECT_DIR "/input/expr.fl";
    char *file_content = fl_file_read(filename);

    if (file_content == FL_NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", filename);
        return 1;
    }

    fl_token_arr token_array;
    fl_bool res = fl_tok_tokenize(file_content, &token_array);

    fl_file_free(file_content);

    if (!res)
    {
        fprintf(stderr, "Failed to fl_tokenize content.\n");
        return 1;
    }

    fl_size old_num = token_array._count;
    fl_opt_optimalize(&token_array);

    // fl_prt_print_tokens_joined(tokens, tokens_num);
    printf("[old] %lu\n", old_num);
    fl_prt_print_token_entries(&token_array);

    fl_ast_node *root;
    fl_ast_gen(&root, &token_array);
    printf("\n");
    fl_prt_print_ast_tree(root, 0);

    fl_token_arr_free(&token_array);

    return 0;
}