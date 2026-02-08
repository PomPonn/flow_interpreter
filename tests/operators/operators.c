#include <stdio.h>

#include "../framework.h"

int main()
{
    __prepare_tokens(token_array);

    fl_ast_node *root = fl_ast_gen(&token_array);

    fl_ast_free(root);
    __free_prepared(token_array);
    return 0;
}