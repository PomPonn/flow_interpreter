#include "preprocessor.h"

#include "shared/common.h"
#include "shared/token.h"

#include <stdlib.h>
#include <string.h>

static fl_size _fl_ppc_optimalize(fl_token **optimalized, fl_token *tokens, fl_size fl_tokens_num)
{
    fl_size opt_size = fl_tokens_num;
    fl_token_type lfl_ast_type = FL_TOKEN_TYPE_NONE;

    for (fl_size i = 0; i < fl_tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
        {
            opt_size--;
            continue;
        }

        if (tokens[i].alias == FL_TOKEN_ALIAS_NL)
        {
            if (lfl_ast_type == FL_TOKEN_TYPE_NONE ||
                lfl_ast_type == FL_TOKEN_TYPE_OPERATOR ||
                lfl_ast_type == FL_TOKEN_TYPE_DELIMITER)
            {
                opt_size--;
            }
        }

        lfl_ast_type = tokens[i].type;
    }

    *optimalized = calloc(opt_size, sizeof(fl_token));

    lfl_ast_type = FL_TOKEN_TYPE_NONE;
    fl_size j = 0;

    for (fl_size i = 0; i < fl_tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
        {
            continue;
        }
        if (tokens[i].alias == FL_TOKEN_ALIAS_NL)
        {
            if (lfl_ast_type == FL_TOKEN_TYPE_NONE ||
                lfl_ast_type == FL_TOKEN_TYPE_OPERATOR ||
                lfl_ast_type == FL_TOKEN_TYPE_DELIMITER)
            {
                continue;
            }
        }

        (*optimalized)[j] = tokens[i];
        (*optimalized)[j].value = _strdup(tokens[i].value);

        lfl_ast_type = tokens[i].type;
        j++;
    }

    return opt_size;
}

void fl_ppc_preprocess(fl_token *tokens, fl_size tokens_num)
{
    fl_token *opt;

    fl_size opt_tnum = _fl_ppc_optimalize(&opt, tokens, tokens_num);

    // FREE OLD FL_TOKENS HERE
}