#include <stdio.h>

#include "tools/file.h"
#include "tools/printer.h"
#include "optimalizer/optimalizer.h"
#include "tokenizer/tokenizer.h"
#include "compiler/ast.h"
#include "tools/profiler.h"

int main()
{
    const char *filename = "../../../../test.fl";
    char *file_content = fl_file_read(filename);

    if (file_content == FL_NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", filename);
        return 1;
    }

    fl_size tokens_num;
    fl_token *tokens = fl_tok_tokenize(file_content, &tokens_num);

    fl_file_free(file_content);

    if (!tokens)
    {
        fprintf(stderr, "Failed to fl_tokenize content.\n");
        return 1;
    }

    fl_size old_num = tokens_num;

    fl_prof_profile(fl_opt_optimalize, tokens, tokens_num, &tokens_num);

    // tokens = fl_opt_optimalize(tokens, tokens_num, &tokens_num);

    // fl_prt_print_tokens_joined(tokens, tokens_num);
    // printf("[old] %lu\n", old_num);
    // fl_prt_print_token_entries(tokens, tokens_num);

    // fl_ast_node *root;
    // fl_ast_gen(&root, tokens, tokens_num);
    // printf("\n");
    // fl_prt_print_ast_tree(root, 1);

    fl_tok_free_tokens(tokens, tokens_num);

    return 0;
}