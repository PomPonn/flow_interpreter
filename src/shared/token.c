#include "shared/token.h"

#include "shared/delims.h"
#include "shared/memory.h"

#include "stdio.h"

// --- DEFINES ---

#define _fl_token_delim_enum(delim_chr) (fl_token_alias)(fl_str_index(FL_ORDERED_DELIMS, delim_chr) + FL_TOKEN_ALIAS_SPACE)

#define _fl_check_var_arr(arr, count, type, val) \
    for (fl_uint i = 0; i < count; i++)          \
        if (val == arr[i])                       \
            return FL_TRUE;                      \
    return FL_FALSE;

// --- STATIC FUNCTION DEFINITIONS ---

static inline fl_bool _fl_token_deduce(fl_token *tok);
static inline fl_bool _fl_token_is_wrapper(fl_token *tok, fl_char wrapper);

// --- IMPLEMENTATIONS ---

fl_char *fl_token_str_as_printable(fl_char *tok)
{
    if (*tok == FL_CHR_NL)
    {
        return "\\n";
    }
    else if (*tok == '\t')
    {
        return "\\t";
    }
    else if (*tok == '\r')
    {
        return "\\r";
    }
    else
    {
        return tok;
    }
}

fl_void fl_token_free(fl_token *token)
{
    if (token->value)
        fl_mem_free(token->value);
}

fl_bool fl_token_alias_in(fl_token *token, fl_token_alias aliases[], fl_uint count)
{
    if (!token)
        return FL_FALSE;

    _fl_check_var_arr(aliases, count, fl_token_alias, token->alias);
}

fl_bool fl_token_type_in(fl_token *token, fl_token_type types[], fl_uint count)
{
    if (!token)
        return FL_FALSE;

    _fl_check_var_arr(types, count, fl_token_type, token->type);
}

fl_bool fl_token_init(fl_token *token, fl_char *value, fl_size line, fl_size column)
{
    token->value = value;
    token->line = line;
    token->column = column;

    return _fl_token_deduce(token);
}

static inline fl_bool _fl_token_deduce(fl_token *tok)
{
    assert(tok != FL_NULL);

    fl_char first_chr = (tok->value) ? *(tok->value) : '\0';

    // is delim
    if (fl_str_find(FL_ORDERED_DELIMS, first_chr))
    {
        tok->type = FL_TOKEN_TYPE_DELIMITER;
        tok->alias = _fl_token_delim_enum(first_chr);
    }
    // is number
    else if (fl_str_is_num(tok->value))
    {
        tok->type = FL_TOKEN_TYPE_LITERAL;
        tok->alias = FL_TOKEN_ALIAS_NUMBER;
    }
    // is comment
    else if (_fl_token_is_wrapper(tok, FL_COMMENT_WRAPPER))
    {
        tok->type = FL_TOKEN_TYPE_COMMENT;
        tok->alias = FL_TOKEN_ALIAS_NONE;
    }
    // is string
    else if (_fl_token_is_wrapper(tok, FL_STRING_WRAPPER))
    {
        tok->type = FL_TOKEN_TYPE_LITERAL;
        tok->alias = FL_TOKEN_ALIAS_STRING;
    }
    // is valid identifier
    else if (!fl_chr_is_digit(*tok->value))
    {
        tok->type = FL_TOKEN_TYPE_IDENTIFIER;
        tok->alias = FL_TOKEN_ALIAS_NONE;
    }
    else
    {
        tok->type = FL_TOKEN_TYPE_NONE;
        tok->alias = FL_TOKEN_ALIAS_NONE;
        return FL_FALSE;
    }

    return FL_TRUE;
}

static inline fl_bool _fl_token_is_wrapper(fl_token *tok, fl_char wrapper)
{
    return tok->value[0] == wrapper && tok->value[fl_str_len(tok->value) - 1] == wrapper;
}

fl_bool fl_token_is_expr_killer(fl_token *token)
{
    return token->alias == FL_TOKEN_ALIAS_NL || token->alias == FL_TOKEN_ALIAS_SEMICOLON;
}

fl_bool fl_token_is_id_like(fl_token *token)
{
    return token->type == FL_TOKEN_TYPE_IDENTIFIER || token->alias == FL_TOKEN_ALIAS_DOT;
}