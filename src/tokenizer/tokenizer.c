#include "tokenizer.h"

#include "tokenizer/delims.h"
#include "shared/str.h"

// ---- DECLARATIONS ----

static inline fl_size _fl_tok_count_tokens(fl_char *str, const fl_cstr_vec *escapes);
static inline fl_bool _fl_tok_handle_number(const fl_char **str);
static inline fl_bool _fl_tok_is_char_escaped(const fl_char *chr, fl_size chr_index);
static inline fl_bool _fl_tok_handle_wrapper(const fl_char **str, fl_size *line_counter, fl_size *column_counter);
static inline fl_char *_fl_tok_handle_whitespace(fl_char *token_start, fl_size *line_counter, fl_size *col_counter);
static fl_char *_fl_tok_next_token(
    fl_char *str,
    const fl_cstr_vec *escapes,
    fl_size *line,
    fl_size *column);
static inline fl_bool _fl_tok_handle_escapement(
    const fl_char **str,
    const fl_cstr_vec *escapes);

// ---- DEFINITIONS ----

// returns FL_FALSE if source has no tokens
fl_bool fl_tok_tokenize(
    fl_char *source,
    fl_token_arr *token_arr,
    const fl_cstr_vec *escapes,
    const fl_char *src_location)
{
    fl_uint count = _fl_tok_count_tokens(source, escapes);
    if (!count)
        return FL_FALSE;

    // Initialize token array
    fl_token_arr_init(token_arr, count);

    // Tokenize the string
    fl_size line, column;
    fl_char *token_str = _fl_tok_next_token(source, escapes, &line, &column);

    for (fl_size index = 0; token_str; index++)
    {
        fl_token_init(
            token_arr->_data + index,
            fl_str_dup(token_str),
            fl_token_location_create(src_location, line, column));

        token_str = _fl_tok_next_token(FL_NULL, escapes, &line, &column);
    }

    return FL_TRUE;
}

static inline fl_bool _fl_tok_handle_escapement(
    const fl_char **str,
    const fl_cstr_vec *escapes)
{
    const fl_char *ptr = *str;

    for (fl_size i = 0; i < escapes->size; i++)
    {
        const fl_char *curr = escapes->data[i];
        const fl_size len = fl_str_len(curr);
        if (fl_str_equal_n(ptr, curr, len))
        {
            *str += len;
            return FL_TRUE;
        }
    }

    return FL_FALSE;
}

static inline fl_bool _fl_tok_handle_number(const fl_char **str)
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
            *str += i;
            return digit_found;
        }
    }

    return FL_FALSE;
}

static inline fl_bool _fl_tok_is_char_escaped(const fl_char *chr, fl_size chr_index)
{
    if (chr_index == 0)
        return FL_FALSE;

    fl_bool res = *(chr - 1) == FL_ESCAPE_CHAR;

    if (chr_index > 1)
        res = res && *(chr - 2) != FL_ESCAPE_CHAR;

    return res;
}

static inline fl_bool _fl_tok_handle_wrapper(const fl_char **str, fl_size *line_counter, fl_size *column_counter)
{
    const fl_char *ptr = *str;

    for (fl_size i = 0; i < fl_str_len(FL_WRAPPERS); i++)
    {
        const fl_char wrapper = FL_WRAPPERS[i];
        ptr = *str;

        if (*ptr != wrapper)
            continue;

        ptr++;

        for (fl_size j = 0; ptr[j]; j++)
        {
            if (ptr[j] == '\n')
            {
                (*line_counter)++;
                (*column_counter) = 0;
            }
            else if (ptr[j] == wrapper && !_fl_tok_is_char_escaped(ptr, j))
            {
                *str = ptr + j + 1;
                return FL_TRUE;
            }

            (*column_counter)++;
        }
    }

    return FL_FALSE;
}

// reduces any following whitespace
static inline fl_char *_fl_tok_handle_whitespace(fl_char *token_start, fl_size *line_counter, fl_size *col_counter)
{
    fl_char *ptr = token_start;

    while (fl_chr_is_any(*ptr, FL_WHITESPACE))
    {
        if (*ptr == '\n')
        {
            (*line_counter)++;
            *col_counter = 0;
        }
        else if (*ptr != '\r')
            (*col_counter)++;

        ptr++;
    }

    return ptr;
}

// finds next token in a string
static fl_char *_fl_tok_next_token(
    fl_char *str,
    const fl_cstr_vec *escapes,
    fl_size *line,
    fl_size *column)
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

    // skip any following whitespace, since it's ignorable
    next_pos = _fl_tok_handle_whitespace(next_pos, &next_line, &next_column);

    if (line != FL_NULL)
        *line = next_line;
    if (column != FL_NULL)
        *column = next_column;

    // start of the current token
    fl_char *token_start = next_pos;

    // return null if there is no content in string
    if (!*token_start)
        return FL_NULL;

    if (_fl_tok_handle_wrapper(&next_pos, &next_line, &next_column))
    {
        escape_char = *next_pos;
        if (*next_pos == '\0')
            next_pos = FL_NULL;
        else
        {
            *next_pos = '\0';
            next_pos++;
        }

        return token_start;
    }

    if (_fl_tok_handle_number(&next_pos))
        goto __std_return;

    // end if is pointing at delim
    if (fl_chr_is_any(*next_pos, FL_ORDERED_DELIMS))
    {
        next_pos++;
        goto __std_return;
    }

    // check if it is an escapement
    if (_fl_tok_handle_escapement(&next_pos, escapes))
        goto __std_return;

    // find ending position for this token
    next_pos = fl_str_find_any(++next_pos, FL_WHITESPACE FL_ORDERED_DELIMS);

    if (!next_pos)
        return token_start;

__std_return:

    escape_char = *next_pos;

    if (*next_pos == '\0')
        next_pos = FL_NULL;
    else
    {
        *next_pos = '\0';
        next_pos++;
    }

    next_column += fl_str_len(token_start);
    return token_start;
}

static inline fl_size _fl_tok_count_tokens(fl_char *str, const fl_cstr_vec *escapes)
{
    fl_size count = 0;

    if (_fl_tok_next_token(str, escapes, FL_NULL, FL_NULL))
        count++;
    else
        return 0;

    while (_fl_tok_next_token(FL_NULL, escapes, FL_NULL, FL_NULL))
        count++;

    return count;
}