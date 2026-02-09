
#include "framework.h"

int main()
{
    __prepare_tokens(token_array);

    printf("%.2fKb", sizeof(fl_ast_node) * 120 / 1024.0);
    return 0;

    fl_ast_node *root = fl_ast_gen(&token_array);

    fl_prt_print_ast_tree(root, 0);

    fl_ast_free(root);
    __free_prepared(token_array);
    return 0;
}