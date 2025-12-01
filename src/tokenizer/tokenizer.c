#include "tokenizer.h"

#include "shared/memory.h"
#include "shared/str.h"

#include <stdlib.h>

// --- STATIC FUNCTION DEFINITIONS ---

static fl_uint _fl_tok_count_tokens(fl_char *str);
static fl_bool _fl_tok_handle_number(const fl_char **str);
static fl_char *_fl_tok_next_token(fl_char *str);
static fl_bool _fl_tok_is_char_escaped(const fl_char *chr, fl_size chr_index);
static fl_bool _fl_tok_handle_wrapper(const fl_char **str);
static fl_bool _fl_tok_handle_escapement(const fl_char **str);

// --- IMPLEMENTATIONS ---

static fl_bool _fl_tok_handle_escapement(const fl_char **str)
{
    if (_fl_tok_handle_wrapper(str))
        return FL_TRUE;

    if (_fl_tok_handle_number(str))
        return FL_TRUE;

    return FL_FALSE;
}

static fl_bool _fl_tok_handle_number(const fl_char **str)
{
    fl_bool dot_found = FL_FALSE;
    fl_bool digit_found = FL_FALSE;
    const fl_char *ptr = *str;

    for (fl_size i = 0; ptr[i]; i++)
    {
        if (ptr[i] == '.')
        {
            if (dot_found || i == 0)
                return FL_FALSE;
            else
            {
                dot_found = FL_TRUE;
                digit_found = FL_FALSE;
            }
        }
        else if (fl_chr_is_digit(ptr[i]))
            digit_found = FL_TRUE;
        else
        {
            *str = ptr + i;
            return digit_found;
        }
    }

    return FL_FALSE;
}

static fl_bool _fl_tok_is_char_escaped(const fl_char *chr, fl_size chr_index)
{
    if (chr_index == 0)
        return FL_FALSE;

    fl_bool res = *(chr - 1) == FL_ESCAPE_CHAR;

    if (chr_index > 1)
        res = res && *(chr - 2) != FL_ESCAPE_CHAR;

    return res;
}

static fl_bool _fl_tok_handle_wrapper(const fl_char **str)
{
    const fl_char *ptr = *str;

    for (fl_size i = 0; i < fl_str_len(FL_WRAPPERS); i++)
    {
        const fl_char wrapper = FL_WRAPPERS[i];
        ptr = *str;

        if (*ptr == wrapper)
        {
            ptr++;

            for (fl_size j = 0; ptr[j]; j++)
            {
                if (*ptr == wrapper && !_fl_tok_is_char_escaped(ptr, j))
                {
                    *str = ptr + 1;
                    return FL_TRUE;
                }

                ptr++;
            }
        }
    }

    return FL_FALSE;
}

static fl_char *_fl_tok_next_token(fl_char *str)
{
    static fl_char *next_pos = FL_NULL;
    static fl_char escape_char = '\0';

    if (str)
    {
        next_pos = str;
        escape_char = '\0';
    }
    else if (next_pos == FL_NULL)
        return FL_NULL;

    if (escape_char)
    {
        next_pos--;
        *next_pos = escape_char;
        escape_char = '\0';
    }

    fl_char *token_start = next_pos;

    if (_fl_tok_handle_escapement(&next_pos))
    {
        goto __return;
    }

    if (fl_chr_is_any(*next_pos, FL_ORDERED_DELIMS))
    {
        next_pos++;
        goto __return;
    }

    next_pos = fl_str_find_any(next_pos + 1, FL_ORDERED_DELIMS);

    if (!next_pos)
    {
        return token_start;
    }

    escape_char = *next_pos;
    goto __return;

__return:

    escape_char = *next_pos;

    if (*next_pos == '\0')
    {
        next_pos = FL_NULL;
    }
    else
    {
        *next_pos = '\0';
        next_pos++;
    }

    return token_start;
}

static fl_uint _fl_tok_count_tokens(fl_char *str)
{
    fl_uint count = 0;
    if (_fl_tok_next_token(str))
        count++;
    else
        return 0;

    while (_fl_tok_next_token(FL_NULL))
        count++;

    return count;
}

fl_token *fl_tok_tokenize(fl_char *str, fl_size *tokens_num)
{
    fl_uint count = _fl_tok_count_tokens(str);

    if (count == 0)
    {
        *tokens_num = 0;
        return FL_NULL;
    }

    // Allocate memory for tokens
    fl_token *tokens = fl_mem_alloc_arr(count, sizeof(fl_token));
    if (tokens == FL_NULL)
        return FL_NULL;

    // Tokenize the string
    fl_char *token = _fl_tok_next_token(str);
    fl_char *last_token = FL_NULL;
    fl_size line = 1, column = 1;

    for (fl_size index = 0; token; index++)
    {
        fl_token_init(tokens + index, fl_str_dup(token));
        tokens[index].line = line;
        tokens[index].column = column;

        last_token = token;
        token = _fl_tok_next_token(FL_NULL);

        if (*last_token == '\n')
        {
            line++;
            column = 1;
        }
        else
        {
            column += token - last_token;
        }
    }

    *tokens_num = count;

    return tokens;
}

void fl_tok_free_tokens(fl_token *tokens, fl_size tokens_num)
{
    if (tokens == FL_NULL)
        return;

    for (fl_size i = 0; i < tokens_num; i++)
        if (tokens[i].value)
            free(tokens[i].value);

    free(tokens);
}