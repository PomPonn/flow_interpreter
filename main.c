#include <stdio.h>

#include "utils/file.h"
#include "preprocessor/preprocessor.h"
#include "tokenizer/tokenizer.h"
#include "compiler/ast.h"

const char *WHITESPACE = " \r\t";

const char *COMMENT_TOKEN = "#";

const char *KEYWORDS[] = {"use", "export"};

const char *DELIMITERS[] = {",", "(", ")", "{", "}",
                            ";", "[", "]", ".", "\n"};

const char *OPERATORS[] = {
    "==",
    "=>",
    "!=",
    "<<",
    "<=",
    ">>",
    "<=",
    "<",
    ">",
    "=",
    "+",
    "-",
    "*",
    "/",
    "%",
    "&&",
    "||",
    "!",
    "&",
    "|",
    "~",
    "?",
    ":",
};

const char *STR_LITERAL_WRAPPERS[] = {"'", "\""};

#define V_N(var) var, (sizeof(var) / sizeof(*var))

void print_tokens(fl_token *tokens, fl_size n)
{
    for (fl_size i = 0; i < n; i++)
    {
        printf("[%zu, %zu] ", tokens[i].line, tokens[i].column);

        if (*tokens[i].value == '\n')
        {
            printf("'\\n' ");
        }
        else
        {
            printf("'%s' ", tokens[i].value);
        }

        printf("%i, %i\n", tokens[i].type, tokens[i].alias);
    }
}

void print_ast_node(fl_ast_node *node)
{
    printf("-> ");
    switch (node->type)
    {
    case FL_AST_NODE_TYPE_BLOCK:
        printf("<block>");
        break;
    case FL_AST_NODE_TYPE_IMPORT:
        printf("<import>");
        break;
    case FL_AST_NODE_TYPE_IDENTIFIER:
        printf("<id> %s", (char *)node->data);
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

    // Tokenize the content
    fl_tok_context context;
    fl_tok_context_init(&context, COMMENT_TOKEN, WHITESPACE, V_N(DELIMITERS), V_N(OPERATORS), V_N(KEYWORDS), V_N(STR_LITERAL_WRAPPERS));

    fl_size tokens_num;
    fl_token *tokens = fl_tok_tokenize(file_content, &context, &tokens_num);

    fl_tok_context_free(&context);

    fl_file_free(file_content);

    if (!tokens)
    {
        fprintf(stderr, "Failed to fl_tokenize content.\n");
        fl_file_free(file_content);
        return 1;
    }

    fl_ppc_preprocess(&tokens, &tokens_num);

    print_tokens(tokens, tokens_num);

    fl_ast_node *root;
    fl_ast_gen(&root, tokens, tokens_num);

    printf("\n");
    print_ast_tree(root, 1);

    fl_tok_free_tokens(tokens, tokens_num);

    return 0;
}