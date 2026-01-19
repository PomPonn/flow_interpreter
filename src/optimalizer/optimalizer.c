#include "optimalizer.h"

#include "shared/token.h"
#include "shared/memory.h"

fl_token *fl_opt_optimalize(fl_token *tokens, fl_size tokens_num, fl_size *opted_num)
{
    fl_token *opted;
    fl_size opted_size = tokens_num;
    fl_token *last_token = FL_NULL;

    for (fl_size i = 0; i < tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
        {
            fl_token_free(tokens + i);
            opted_size--;
        }
        else if (fl_token_alias_in(tokens + i, fl_token_alias_arr(FL_TOKEN_ALIAS_NL, FL_TOKEN_ALIAS_SPACE)) && (!last_token || fl_token_type_in(last_token, fl_token_type_arr(FL_TOKEN_TYPE_DELIMITER))))
        {
            fl_token_free(tokens + i);
            opted_size--;
        }
        else
        {
            last_token = tokens + i;
        }
    }

    opted = fl_mem_alloc_arr(opted_size, sizeof(fl_token));
    last_token = FL_NULL;

    for (fl_size i = 0, j = 0; i < tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
            continue;
        else if (fl_token_alias_in(tokens + i, fl_token_alias_arr(FL_TOKEN_ALIAS_NL, FL_TOKEN_ALIAS_SPACE)) && (!last_token || fl_token_type_in(last_token, fl_token_type_arr(FL_TOKEN_TYPE_DELIMITER))))
            continue;

        last_token = tokens + i;
        opted[j++] = tokens[i];
    }

    *opted_num = opted_size;
    fl_mem_free(tokens);
    return opted;
}