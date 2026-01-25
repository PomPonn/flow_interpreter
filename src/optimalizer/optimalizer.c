#include "optimalizer.h"

#include "shared/token.h"
#include "shared/memory.h"

static inline fl_token *_fl_opt_peek_token(fl_token *tokens, fl_size i, fl_size tokens_num)
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
    fl_token_type last_type = FL_TOKEN_TYPE_NONE;

    for (fl_size i = 0; i < tokens_num; i++)
    {
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
        else if (tokens[i].alias == FL_TOKEN_ALIAS_SPACE)
        {
            peek = _fl_opt_peek_token(tokens, i, tokens_num);

            if (!peek || peek->alias != FL_TOKEN_ALIAS_DOT)
            {
                fl_token_free(tokens + i);
                opted_size--;
            }
        }
        else
        {
            last_type = tokens[i].type;
        }
    }

    opted = fl_mem_alloc_arr(opted_size, sizeof(fl_token));
    last_type = FL_TOKEN_TYPE_NONE;

    for (fl_size i = 0, j = 0; i < tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
            continue;
        else if (tokens[i].alias == FL_TOKEN_ALIAS_NL && (last_type == FL_TOKEN_TYPE_DELIMITER || last_type == FL_TOKEN_TYPE_NONE))
            continue;
        else if (tokens[i].alias == FL_TOKEN_ALIAS_SPACE)
        {
            peek = _fl_opt_peek_token(tokens, i, tokens_num);

            if (!peek || peek->alias != FL_TOKEN_ALIAS_DOT)
                continue;
        }

        last_type = tokens[i].type;
        opted[j++] = tokens[i];
    }

    *opted_num = opted_size;
    fl_mem_free(tokens);
    return opted;
}