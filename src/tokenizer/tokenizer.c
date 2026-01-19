#include "tokenizer.h"

#include "shared/memory.h"
#include "shared/str.h"
#include "shared/delims.h"

#include <assert.h>

// --- STATIC FUNCTION DEFINITIONS ---

static fl_uint _fl_tok_count_tokens(fl_char *str);
static fl_bool _fl_tok_handle_number(const fl_char **str);
static fl_char *_fl_tok_next_token(fl_char *str, fl_size *line, fl_size *column);
static fl_bool _fl_tok_is_char_escaped(const fl_char *chr, fl_size chr_index);
static fl_bool _fl_tok_handle_wrapper(const fl_char **str);
static fl_bool _fl_tok_handle_escapement(const fl_char **str);
static fl_char *_fl_tok_handle_whitespace(fl_char *token_start, fl_char **next_pos, fl_size *nls_counter);

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

// returns next token if current char is whitespace and reduces any following whitespace to a single delimeter
// returned token is a static string
// otherwise returns FL_NULL
static fl_char *_fl_tok_handle_whitespace(fl_char *token_start, fl_char **next_pos, fl_size *nls_counter)
{
    fl_char *ptr = token_start + 1;

    if (*token_start == FL_NL || (*token_start == '\r' && *(token_start + 1) == FL_NL))
    {
        while (fl_chr_is_any(*ptr, FL_WHITESPACE))
        {
            if (*ptr == FL_NL)
                (*nls_counter)++;
            ptr++;
        }
        *next_pos = ptr;

        return "\n";
    }
    else if (*token_start == ' ' || *token_start == '\t' || *token_start == '\r')
    {
        while (fl_chr_is_any(*ptr, " \t\r"))
        {
            ptr++;
        }
        *next_pos = ptr;

        return " ";
    }

    return FL_NULL;
}

// find next token in a string and returns string copy of it
static fl_char *_fl_tok_next_token(fl_char *str, fl_size *line, fl_size *column)
{
    // start of the next token (used on next function call)
    static fl_char *next_pos = FL_NULL;
    static fl_size next_line = 0;
    static fl_size next_column = 0;
    static fl_char escape_char = '\0';

    if (str)
    {
        // reset static variables to operate on new string
        next_pos = str;
        escape_char = '\0';
        next_line = 0;
        next_column = 0;
    }
    else if (next_pos == FL_NULL)
        return FL_NULL;

    // fix string from last iteration
    if (escape_char)
    {
        next_pos--;
        *next_pos = escape_char;
        escape_char = '\0';
    }

    if (line != FL_NULL)
        *line = next_line;
    if (column != FL_NULL)
        *column = next_column;

    // start of the current token
    fl_char *token_start = next_pos;
    // potential whitespace delimeter
    fl_char *ws_delim;

    // reduce any whitespace to single delimeter
    // - space or newline
    if (ws_delim = _fl_tok_handle_whitespace(token_start, &next_pos, &next_line))
    {
        next_column = next_pos - token_start - 1;
        escape_char = '\0';

        if (*ws_delim == FL_NL)
            next_column = 0;

        return ws_delim;
    }

    if (_fl_tok_handle_escapement(&next_pos))
    {
        goto __std_return;
    }

    if (fl_chr_is_any(*next_pos, FL_ORDERED_DELIMS))
    {
        next_pos++;
        goto __std_return;
    }

    next_pos = fl_str_find_any(next_pos + 1, FL_ORDERED_DELIMS);

    if (!next_pos)
        return token_start;

__std_return:

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

    next_column += fl_str_len(token_start);

    return token_start;
}

static fl_uint _fl_tok_count_tokens(fl_char *str)
{
    fl_uint count = 0;

    if (_fl_tok_next_token(str, FL_NULL, FL_NULL))
        count++;
    else
        return 0;

    while (_fl_tok_next_token(FL_NULL, FL_NULL, FL_NULL))
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
    fl_size line, column;
    fl_char *token = _fl_tok_next_token(str, &line, &column);

    for (fl_size index = 0; token; index++)
    {
        assert(fl_token_init(tokens + index, fl_str_dup(token), line, column));
        token = _fl_tok_next_token(FL_NULL, &line, &column);
    }

    *tokens_num = count;

    return tokens;
}

fl_void fl_tok_free_tokens(fl_token *tokens, fl_size tokens_num)
{
    if (tokens == FL_NULL)
        return;

    for (fl_size i = 0; i < tokens_num; i++)
        fl_token_free(tokens + i);

    fl_mem_free(tokens);
}