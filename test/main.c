#include "compiler/compiler.h"

#include <stdio.h>

int main()
{
    fl_context cnt;
    fl_context_init(&cnt);

    // fl_meta_register(&cnt);

    fl_comp_compile(_TEST_INPUT_PATH, &cnt);

    return 0;
}

// int main()
// {
//     __prepare_tokens(token_array);

//     fl_ast_node *root = fl_ast_gen(&token_array);

//     fl_prt_print_ast_tree(root, 0);

//     fl_ast_free(root);
//     __free_prepared(token_array);
//     return 0;
// }