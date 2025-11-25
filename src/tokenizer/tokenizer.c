#include "tokenizer.h"

#include "shared/memory.h"
#include "shared/str.h"

#include <string.h>
#include <stdlib.h>

static fl_char *_tok_trim_left(fl_char *str, const fl_char *to_trim)
{
    if (str == FL_NULL || to_trim == FL_NULL)
        return str;

    while (*str && fl_str_has(to_trim, *str))
        str++;

    return str;
}

static fl_bool _tok_escape_next(const fl_char *str, fl_tok_context *cnt,
                                fl_size *escape_len)
{
    for (fl_size i = 0; i < cnt->escapes_num; i++)
    {
        fl_size len = fl_str_len(cnt->escapes[i]);
        if (fl_str_equal_n(str, cnt->escapes[i], len))
        {
            if (escape_len)
                *escape_len = len;
            return 1;
        }
    }

    return 0;
}

static fl_bool _tok_number_next(const fl_char *str, fl_tok_context *cnt,
                                fl_size *num_len)
{
    fl_bool dot_found = FL_FALSE;
    fl_bool digit_found = FL_FALSE;

    for (fl_size i = 0; str[i]; i++)
    {
        if (str[i] == '.')
        {
            if (dot_found || i == 0)
                return FL_FALSE;
            else
            {
                dot_found = FL_TRUE;
                digit_found = FL_FALSE;
            }
        }
        else if (fl_chr_is_digit(str[i]))
            digit_found = FL_TRUE;
        else
        {
            *num_len = i;
            return digit_found;
        }
    }

    return FL_FALSE;
}

static fl_bool _tok_string_wrapper_next(const fl_char *str, fl_tok_context *cnt,
                                        fl_size *string_len)
{
    for (fl_size i = 0; i < cnt->string_wrappers_num; i++)
    {
        fl_size len = fl_str_len(cnt->string_wrappers[i]);
        if (fl_str_equal_n(str, cnt->string_wrappers[i], len))
        {
            const fl_char *ptr = str + len;
            while (*ptr && !fl_str_equal_n(ptr, cnt->string_wrappers[i], len))
                ptr++;
            if (*ptr)
                ptr++;
            *string_len = ptr - str;
            return 1;
        }
    }

    return 0;
}

static fl_bool _tok_comment_wrapper_next(const fl_char *str, fl_tok_context *cnt,
                                         fl_size *comment_len)
{
    fl_size len = strlen(cnt->comment_wrapper);
    if (strncmp(str, cnt->comment_wrapper, len) == 0)
    {
        const fl_char *ptr = str + len;
        while (*ptr && strncmp(ptr, cnt->comment_wrapper, len) != 0)
            ptr++;
        if (*ptr)
            ptr++;
        *comment_len = ptr - str;
        return 1;
    }

    return 0;
}

static fl_char *_tok_next_token(fl_char *str, fl_tok_context *cnt)
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

    // next_pos = _tok_trim_left(next_pos, cnt->ws);
    // if (*next_pos == '\0')
    //     return FL_NULL;

    fl_char *token_start = next_pos;
    fl_size skip_len = 0;

    // Check for number
    if (_tok_number_next(next_pos, cnt, &skip_len))
    {
        next_pos += skip_len;
        escape_char = *next_pos;

        goto __return;
    }

    // Check for string wrappers
    if (_tok_string_wrapper_next(next_pos, cnt, &skip_len))
    {
        next_pos += skip_len;
        escape_char = *next_pos;

        goto __return;
    }

    // Check for comment wrapper
    if (_tok_comment_wrapper_next(next_pos, cnt, &skip_len))
    {
        next_pos += skip_len;
        escape_char = *next_pos;

        goto __return;
    }

    // Check for token to escape
    if (_tok_escape_next(next_pos, cnt, &skip_len))
    {
        next_pos += skip_len;
        escape_char = *next_pos;
        goto __return;
    }

    fl_char *next_delim = strpbrk(next_pos, cnt->ws);
    fl_char *next_escape = strpbrk(next_pos, cnt->escapes_peek);

    if (next_escape == FL_NULL && next_delim == FL_NULL)
    {
        next_pos = FL_NULL;
        return token_start;
    }

    if (next_delim == FL_NULL)
        next_pos = next_escape;
    else if (next_escape == FL_NULL)
        next_pos = next_delim;
    else
        next_pos = min(next_escape, next_delim);

    if (next_escape == next_pos)
    {
        escape_char = *next_pos;
        goto __return;
    }

__return:
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

static fl_uint _tok_count_tokens(fl_char *str, fl_tok_context *cnt)
{
    fl_uint count = 0;
    if (_tok_next_token(str, cnt))
        count++;
    else
        return 0;

    while (_tok_next_token(FL_NULL, cnt))
        count++;

    return count;
}

fl_token *fl_tok_tokenize(fl_char *str, fl_tok_context *context, fl_size *tokens_num)
{
    // validate arguments
    if (str == FL_NULL, tokens_num == FL_NULL)
        return FL_NULL;

    // count tokens
    fl_char *str_copy = fl_str_dup(str);
    fl_uint count = _tok_count_tokens(str_copy, context);
    free(str_copy);

    if (count == 0)
    {
        *tokens_num = 0;
        return FL_NULL;
    }

    // Allocate memory for tokens
    fl_token *tokens = calloc(count, sizeof(fl_token));
    if (tokens == FL_NULL)
        return FL_NULL;

    str_copy = fl_str_dup(str);

    // Tokenize the string
    fl_char *token = _tok_next_token(str_copy, context);
    fl_char *last_token = FL_NULL;
    fl_int line = 1, column = 1;
    for (fl_size index = 0; token; index++)
    {
        fl_token_init((tokens + index), fl_str_dup(token), context);
        tokens[index].line = line;
        tokens[index].column = column;

        last_token = token;
        token = _tok_next_token(FL_NULL, context);

        if (*last_token == '\n')
        {
            line++;
            column = 1;
        }
        else
        {
            column += (fl_int)(token - last_token);
        }
    }

    free(str_copy);

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
