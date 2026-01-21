#include "optimalizer.h"

#include "shared/token.h"
#include "shared/memory.h"

static fl_token *fl_opt_peek_token(fl_token *tokens, fl_size i, fl_size tokens_num)
{
    if (++i < tokens_num)
    {
        return tokens + i;
    }
    else
    {
        static fl_token empty;
        fl_token_init(&empty, FL_NULL, 0, 0);

        return &empty;
    }
}

fl_token *fl_opt_optimalize(fl_token *tokens, fl_size tokens_num, fl_size *opted_num)
{
    fl_token *opted;
    fl_size opted_size = tokens_num;
    fl_token *peek;
    fl_token *last_token;
    fl_token_type last_type = FL_TOKEN_TYPE_NONE;

    for (fl_size i = 0; i < tokens_num; i++)
    {
        peek = fl_opt_peek_token(tokens, i, tokens_num);

        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
        {
            fl_token_free(tokens + i);
            opted_size--;
        }
        else if (tokens[i].alias == FL_TOKEN_ALIAS_NL && (last_type == FL_TOKEN_TYPE_DELIMITER || last_type == FL_TOKEN_TYPE_NONE))
        {
            fl_token_free(tokens + i);
            opted_size--;
        }
        else if (tokens[i].alias == FL_TOKEN_ALIAS_SPACE && !fl_token_is_id_like(last_token) && !fl_token_is_id_like(peek))
        {
            fl_token_free(tokens + i);
            opted_size--;
        }
        else
        {
            last_token = tokens + i;
            last_type = last_token->type;
        }
    }

    opted = fl_mem_alloc_arr(opted_size, sizeof(fl_token));
    last_token = FL_NULL;
    last_type = FL_TOKEN_TYPE_NONE;

    for (fl_size i = 0, j = 0; i < tokens_num; i++)
    {
        peek = fl_opt_peek_token(tokens, i, tokens_num);

        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
            continue;
        else if (tokens[i].alias == FL_TOKEN_ALIAS_NL && (last_type == FL_TOKEN_TYPE_DELIMITER || last_type == FL_TOKEN_TYPE_NONE))
            continue;
        else if (tokens[i].alias == FL_TOKEN_ALIAS_SPACE && !fl_token_is_id_like(last_token) && !fl_token_is_id_like(peek))
            continue;

        last_token = tokens + i;
        last_type = last_token->type;
        opted[j++] = tokens[i];
    }

    *opted_num = opted_size;
    fl_mem_free(tokens);
    return opted;
}