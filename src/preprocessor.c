#include "preprocessor.h"

#include <stdlib.h>
#include <string.h>
// TEMP
#include <stdio.h>

#include "common.h"
#include "str.h"

#define IS_TOK(token, val) str_equal(token##->value, val)

static void ppc_token_deduce_type(inp_token *token)
{
    if (token == NULL)
        return;

    if (inp_token_is_delimiter(*token))
    {
        token->type = COMP_TOKEN_TYPE_DELIMITER;
    }
    else if (inp_token_is_keyword(*token))
    {
        token->type = COMP_TOKEN_TYPE_KEYWORD;
    }
    else if (inp_token_is_operator(*token))
    {
        token->type = COMP_TOKEN_TYPE_OPERATOR;
    }
    else if (inp_token_is_comment(*token))
    {
        token->type = COMP_TOKEN_TYPE_COMMENT;
    }
    else if (inp_token_is_literal(*token))
    {
        token->type = COMP_TOKEN_TYPE_LITERAL;
    }
    else
    {
        token->type = COMP_TOKEN_TYPE_IDENTIFIER;
    }
}

static void ppc_token_deduce_alias(inp_token *token)
{
    if (token == NULL)
        return;

    if (inp_token_is_comment(*token) || inp_token_is_literal(*token))
    {
        token->alias = COMP_TOKEN_ALIAS_NONE; // No alias for comments or literals
        return;
    }
    else if (inp_token_is_keyword(*token))
    {
        if (IS_TOK(token, "import"))
        {
            token->alias = COMP_TOKEN_ALIAS_IMPORT;
        }
        else if (IS_TOK(token, "export"))
        {
            token->alias = COMP_TOKEN_ALIAS_EXPORT;
        }
    }
    else if (inp_token_is_delimiter(*token))
    {
        if (IS_TOK(token, ","))
        {
            token->alias = COMP_TOKEN_ALIAS_COMMA;
        }
        else if (IS_TOK(token, "("))
        {
            token->alias = COMP_TOKEN_ALIAS_LPAREN;
        }
        else if (IS_TOK(token, ")"))
        {
            token->alias = COMP_TOKEN_ALIAS_RPAREN;
        }
        else if (IS_TOK(token, "{"))
        {
            token->alias = COMP_TOKEN_ALIAS_LBRACE;
        }
        else if (IS_TOK(token, "}"))
        {
            token->alias = COMP_TOKEN_ALIAS_RBRACE;
        }
        else if (IS_TOK(token, ";"))
        {
            token->alias = COMP_TOKEN_ALIAS_SEMICOLON;
        }
        else if (IS_TOK(token, "["))
        {
            token->alias = COMP_TOKEN_ALIAS_LBRACKET;
        }
        else if (IS_TOK(token, "]"))
        {
            token->alias = COMP_TOKEN_ALIAS_RBRACKET;
        }
        else if (IS_TOK(token, "."))
        {
            token->alias = COMP_TOKEN_ALIAS_DOT;
        }
        else if (IS_TOK(token, "\n"))
        {
            token->alias = COMP_TOKEN_ALIAS_NL;
        }
    }
    else if (inp_token_is_operator(*token))
    {
        if (IS_TOK(token, "+"))
        {
            token->alias = COMP_TOKEN_ALIAS_ADD;
        }
        else if (IS_TOK(token, "-"))
        {
            token->alias = COMP_TOKEN_ALIAS_SUB;
        }
        else if (IS_TOK(token, "*"))
        {
            token->alias = COMP_TOKEN_ALIAS_MUL;
        }
        else if (IS_TOK(token, "/"))
        {
            token->alias = COMP_TOKEN_ALIAS_DIV;
        }
        else if (IS_TOK(token, "%"))
        {
            token->alias = COMP_TOKEN_ALIAS_MOD;
        }
        else if (IS_TOK(token, "&&"))
        {
            token->alias = COMP_TOKEN_ALIAS_AND;
        }
        else if (IS_TOK(token, "||"))
        {
            token->alias = COMP_TOKEN_ALIAS_OR;
        }
        else if (IS_TOK(token, "<"))
        {
            token->alias = COMP_TOKEN_ALIAS_LT;
        }
        else if (IS_TOK(token, ">"))
        {
            token->alias = COMP_TOKEN_ALIAS_GT;
        }
        else if (IS_TOK(token, "<="))
        {
            token->alias = COMP_TOKEN_ALIAS_LTE;
        }
        else if (IS_TOK(token, ">="))
        {
            token->alias = COMP_TOKEN_ALIAS_GTE;
        }
        else if (IS_TOK(token, "=="))
        {
            token->alias = COMP_TOKEN_ALIAS_EQ;
        }
        else if (IS_TOK(token, "!="))
        {
            token->alias = COMP_TOKEN_ALIAS_NEQ;
        }
        else if (IS_TOK(token, "="))
        {
            token->alias = COMP_TOKEN_ALIAS_ASSIGN;
        }
        else if (IS_TOK(token, "!"))
        {
            token->alias = COMP_TOKEN_ALIAS_NOT;
        }
        else if (IS_TOK(token, "&"))
        {
            token->alias = COMP_TOKEN_ALIAS_BIT_AND;
        }
        else if (IS_TOK(token, "|"))
        {
            token->alias = COMP_TOKEN_ALIAS_BIT_OR;
        }
        else if (IS_TOK(token, "~"))
        {
            token->alias = COMP_TOKEN_ALIAS_BIT_XOR;
        }
        else if (IS_TOK(token, "<<"))
        {
            token->alias = COMP_TOKEN_ALIAS_BIT_LSHIFT;
        }
        else if (IS_TOK(token, ">>"))
        {
            token->alias = COMP_TOKEN_ALIAS_BIT_RSHIFT;
        }
        else if (IS_TOK(token, "?"))
        {
            token->alias = COMP_TOKEN_ALIAS_IF;
        }
        else if (IS_TOK(token, ":"))
        {
            token->alias = COMP_TOKEN_ALIAS_COLON;
        }
        else if (IS_TOK(token, "=>"))
        {
            token->alias = COMP_TOKEN_ALIAS_LAMBDA;
        }
    }
    else
    {
        token->alias = COMP_TOKEN_ALIAS_NONE; // Default alias for identifiers
    }
}

static size_t ppc_optimalize(inp_token **optimalized, inp_token *tokens, size_t tokens_num)
{
    size_t opt_size = tokens_num;
    int8_t nl_last = 0;

    for (size_t i = 0; i < tokens_num; i++)
    {
        if (tokens[i].type == COMP_TOKEN_TYPE_COMMENT)
        {
            opt_size--;
            continue;
        }

        if (tokens[i].alias == COMP_TOKEN_ALIAS_NL)
        {
            if (nl_last)
            {
                opt_size -= 1;
            }

            nl_last = 1;
        }
        else
        {
            nl_last = 0;
        }
    }

    *optimalized = calloc(opt_size, sizeof(inp_token));

    size_t j = 0;
    for (size_t i = 0; i < tokens_num; i++)
    {
        if (tokens[i].type == COMP_TOKEN_TYPE_COMMENT)
        {
            continue;
        }
        if (tokens[i].alias == COMP_TOKEN_ALIAS_NL)
        {
            if (nl_last)
            {
                nl_last = 0;
                continue;
            }
            else
            {
                nl_last = 1;
            }
        }

        (*optimalized)[j] = tokens[i];
        (*optimalized)[j].value = _strdup(tokens[i].value);
        j++;
    }

    return opt_size;
}

void ppc_preprocess(inp_token *tokens, size_t tokens_num)
{
    for (size_t i = 0; i < tokens_num; i++)
    {
        if (tokens[i].value == NULL)
            continue;
        ppc_token_deduce_type(&tokens[i]);
        ppc_token_deduce_alias(&tokens[i]);
    }

    inp_token *opt;
    for (size_t i = 0; i < tokens_num; i++)
    {
        // printf("[%zu, %zu] `%s` %i %i\n", opt[i].line, opt[i].column,
        //        opt[i].value, opt[i].type, opt[i].alias);
        printf("%s", tokens[i].value);
    }

    size_t opt_tnum = ppc_optimalize(&opt, tokens, tokens_num);

    // FREE OLD TOKENS HERE

    // print temporarily
    for (size_t i = 0; i < opt_tnum; i++)
    {
        // printf("[%zu, %zu] `%s` %i %i\n", opt[i].line, opt[i].column,
        //        opt[i].value, opt[i].type, opt[i].alias);
        printf("%s", opt[i].value);
    }
}