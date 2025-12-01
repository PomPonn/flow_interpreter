#include "optimalizer.h"

#include "shared/token.h"
#include "shared/memory.h"

#include <string.h>

static fl_size _fl_ppc_optimalize(fl_token **optimalized, fl_token *tokens, fl_size fl_tokens_num)
{
    fl_size opt_size = fl_tokens_num;
    fl_token_type last_type = FL_TOKEN_TYPE_NONE;

    for (fl_size i = 0; i < fl_tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
        {
            opt_size--;
            continue;
        }

        if (tokens[i].alias == FL_TOKEN_ALIAS_NL)
        {
            if (last_type == FL_TOKEN_TYPE_NONE ||
                last_type == FL_TOKEN_TYPE_DELIMITER)
            {
                opt_size--;
            }
        }

        last_type = tokens[i].type;
    }

    *optimalized = fl_mem_alloc_arr(opt_size, sizeof(fl_token));

    last_type = FL_TOKEN_TYPE_NONE;
    fl_size j = 0;

    for (fl_size i = 0; i < fl_tokens_num; i++)
    {
        if (tokens[i].type == FL_TOKEN_TYPE_COMMENT)
        {
            continue;
        }
        if (tokens[i].alias == FL_TOKEN_ALIAS_NL)
        {
            if (last_type == FL_TOKEN_TYPE_NONE ||
                last_type == FL_TOKEN_TYPE_DELIMITER)
            {
                continue;
            }
        }

        (*optimalized)[j] = tokens[i];
        (*optimalized)[j].value = fl_str_dup(tokens[i].value);

        last_type = tokens[i].type;
        j++;
    }

    return opt_size;
}

void fl_ppc_preprocess(fl_token **tokens, fl_size *tokens_num)
{
    fl_token *opt;
    *tokens_num = _fl_ppc_optimalize(&opt, *tokens, *tokens_num);

    fl_mem_free(*tokens);
    *tokens = opt;
}

void fl_opt_optimalize_source(fl_char **src)
{
    fl_size new_len = fl_str_len(*src);
    fl_char *ptr = *src;
    fl_uint check_dist = 1;

    // count new length
    while (*(ptr + check_dist))
    {
        // ignore caret returns, and go back to check if there is
        // optimalization without it
        if (*ptr == '\r')
        {
            new_len--;
            ptr--;
            check_dist++;
            continue;
        }
        // exclude duplicates
        else if (fl_chr_is_any(*ptr, FL_WHITESPACE) && *ptr == *(ptr + check_dist))
        {
            new_len--;
        }

        ptr += check_dist;
        check_dist = 1;
    }

    fl_char *new_src = fl_mem_alloc_arr(sizeof(fl_char), new_len + 1);
    new_src[new_len] = '\0';
    fl_size i = 0;

    check_dist = 1;
    ptr = *src;

    // fill new array
    while (*(ptr + check_dist))
    {
        // ignore caret returns, and go back to check if there is
        // optimalization without it
        if (*ptr == '\r')
        {
            ptr--;
            check_dist++;
            continue;
        }
        // exclude duplicates
        else if (fl_chr_is_any(*ptr, FL_WHITESPACE) && *ptr == *(ptr + check_dist))
        {
            ptr += check_dist + 1;
            check_dist = 1;
            continue;
        }
        else if (check_dist > 1)
        {
            ptr += check_dist;
            check_dist = 1;
        }

        new_src[i] = *ptr;
        ptr += check_dist;
        i++;
    }

    new_src[i] = *ptr;

    fl_mem_free(*src);
    *src = new_src;
}