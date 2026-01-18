#include <stdio.h>

#include "utils/file.h"
#include "optimalizer/optimalizer.h"
#include "tokenizer/tokenizer.h"
#include "compiler/ast.h"

// const char *WHITESPACE = " \r\t";

// const char *COMMENT_TOKEN = "#";

const char *wrappers = "'\"#";

// const char *KEYWORDS[] = {"use", "export"};
// const char *KEYWORDS[] = {"_w_a_d"};

const char *DELIMITERS = ",(){}[].\n ";

// const char *OPERATORS[] = {
//     "==",
//     "=>",
//     "!=",
//     "<<",
//     "<=",
//     ">>",
//     "<=",
//     "<",
//     ">",
//     "=",
//     "+",
//     "-",
//     "*",
//     "/",
//     "%",
//     "&&",
//     "||",
//     "!",
//     "&",
//     "|",
//     "~",
//     "?",
//     ":",
// };

const char *STR_LITERAL_WRAPPERS[] = {"'", "\""};

#define V_N(var) var, (sizeof(var) / sizeof(*var))

void print_tokens(fl_token *tokens, fl_size n)
{
    for (fl_size i = 0; i < n; i++)
    {
        printf("[%zu, %zu] ", tokens[i].line, tokens[i].column);

        printf("'%s'\n", fl_token_str_as_printable(tokens[i].value));

        // printf("%i, %i\n", tokens[i].type, tokens[i].alias);
    }
}

void print_ast_node(fl_ast_node *node)
{
    printf("> ");
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_ROOT:
        printf("<root>");
        break;
    case FL_AST_NODE_TYPE_BLOCK:
        printf("<block>");
        break;
    case FL_AST_NODE_TYPE_IMPORT:
        printf("<import>");
        break;
    case FL_AST_NODE_TYPE_TYPE:
        printf("<type> [%u]", ((fl_ast_type_node_info *)node->data)->dimensions);
        break;
    case FL_AST_NODE_TYPE_STATEMENT:
        printf("<statement>");
        break;
    case FL_AST_NODE_TYPE_FUNCTION:
        printf("<function>");
        break;
    case FL_AST_NODE_TYPE_FUNC_HEADER:
        printf("<header>");
        break;
    case FL_AST_NODE_TYPE_EXPRESSION:
        printf("<expression>");
        break;
    case FL_AST_NODE_TYPE_LITERAL:
        printf("<literal> '%s'", (fl_char *)node->data);
        break;
    case FL_AST_NODE_TYPE_IDENTIFIER:
        printf("<id> '%s'", (fl_char *)node->data);
        break;
    case FL_AST_NODE_TYPE_ID_CHAIN:
        printf("<id chain>");
        break;
    case FL_AST_NODE_TYPE_OPERATOR:
        printf("<operator> '%s'", (fl_char *)node->data);
        break;
    default:
        printf("<node>");
        break;
    }
}

void print_ast_tree(fl_ast_node *root, fl_ushort nest_level)
{
    for (fl_ushort i = 0; i < nest_level - 1; i++)
    {
        printf("  ");
    }

    print_ast_node(root);

    fl_ast_node *node = root->children;

    if (node)
        printf(":\n");
    else
        printf("\n");

    while (node)
    {
        print_ast_tree(node, nest_level + 1);
        node = node->next;
    }
}

int main()
{
    const char *filename = "../../../../test.fl";
    char *file_content = fl_file_read(filename);

    if (file_content == FL_NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", filename);
        return 1;
    }

    // fl_opt_optimalize_source(&file_content);

    fl_size tokens_num;
    fl_token *tokens = fl_tok_tokenize(file_content, &tokens_num);

    // fl_tok_context_free(&context);

    fl_file_free(file_content);

    if (!tokens)
    {
        fprintf(stderr, "Failed to fl_tokenize content.\n");
        return 1;
    }

    // fl_ppc_preprocess(&tokens, &tokens_num);

    print_tokens(tokens, tokens_num);

    // fl_ast_node *root;
    // fl_ast_gen(&root, tokens, tokens_num);

    printf("\n");
    // print_ast_tree(root, 1);

    fl_tok_free_tokens(tokens, tokens_num);

    return 0;
}