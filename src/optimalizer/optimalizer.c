#include "optimalizer.h"

#include "shared/token.h"
#include "shared/memory.h"

fl_token *fl_opt_optimalize(fl_token *tokens, fl_size tokens_num, fl_size *opted_num)
{
    fl_token *opted;
    fl_size opted_size = tokens_num;
    fl_token_type last_type = FL_TOKEN_TYPE_NONE;

    for (fl_size i = 0; i < tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
        {
            fl_token_free(tokens + i);
            opted_size--;
            continue;
        }

        if (tokens[i].alias == FL_TOKEN_ALIAS_NL)
        {
            if (last_type == FL_TOKEN_TYPE_NONE ||
                last_type == FL_TOKEN_TYPE_DELIMITER)
            {
                fl_token_free(tokens + i);
                opted_size--;
            }
        }

        last_type = tokens[i].type;
    }

    opted = fl_mem_alloc_arr(opted_size, sizeof(fl_token));
    last_type = FL_TOKEN_TYPE_NONE;

    for (fl_size i = 0, j = 0; i < tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
            continue;

        if (tokens[i].alias == FL_TOKEN_ALIAS_NL)
        {
            if (last_type == FL_TOKEN_TYPE_NONE ||
                last_type == FL_TOKEN_TYPE_DELIMITER)
            {
                continue;
            }
        }

        last_type = tokens[i].type;
        opted[j++] = tokens[i];
    }

    *opted_num = opted_size;
    fl_mem_free(tokens);
    return opted;
}