/**
 * ---------- token.c ------------------
 *
 * Implements tokenizer token logic.
 *
 * -------------------------------------
 */

#include "token.h"

// ---------- DEPENDENCIES ----------

#include "shared/str.h"
#include "tokenizer/delims.h"
#include "flow/data_type.h"

// ---------- MACROS -------------

#define _fl_token_delim_enum(delim_chr) (fl_token_alias)(fl_str_index(FL_ORDERED_DELIMS, delim_chr) + 1)

// ---------- DECLARATIONS  ----------

FL_INLINE_FN fl_bool _fl_token_deduce(fl_token *tok);
FL_INLINE_FN fl_bool _fl_token_is_wrapper(fl_token *tok, fl_char wrapper);

// ---------- DEFINITIONS ----------

fl_char *fl_token_as_printable(fl_char *tok)
{
    if (*tok == '\n')
        return "\\n";
    else if (*tok == '\t')
        return "\\t";
    else if (*tok == '\r')
        return "\\r";
    else
        return tok;
}

fl_bool fl_token_is_delim(fl_token *token)
{
    return fl_chr_is_any(*token->value, FL_ORDERED_DELIMS);
}

fl_bool fl_token_init(fl_token *token, fl_char *value, fl_token_location location)
{
    token->value = value;
    token->location = location;

    return _fl_token_deduce(token);
}

FL_INLINE_FN fl_bool _fl_token_deduce(fl_token *tok)
{
    assert(tok != FL_NULL);

    fl_char first_chr = (tok->value) ? *(tok->value) : '\0';
    fl_bool floating = FL_FALSE;

    // is delim
    if (fl_str_find(FL_ORDERED_DELIMS, first_chr))
    {
        tok->type = FL_TOKEN_TYPE_DELIMITER;
        tok->alias = _fl_token_delim_enum(first_chr);
    }
    // is number
    else if (fl_str_is_num(tok->value, &floating))
    {
        tok->type = FL_TOKEN_TYPE_LITERAL;
        tok->alias = floating ? FL_DATA_TYPE_FLOAT : FL_DATA_TYPE_INT;
    }
    // is comment
    else if (_fl_token_is_wrapper(tok, FL_COMMENT_WRAPPER))
    {
        tok->type = FL_TOKEN_TYPE_COMMENT;
        tok->alias = FL_TOKEN_ALIAS_COMMENT;
    }
    // is string
    else if (_fl_token_is_wrapper(tok, FL_STRING_WRAPPER))
    {
        tok->type = FL_TOKEN_TYPE_LITERAL;
        tok->alias = FL_DATA_TYPE_STRING;
    }
    // is valid identifier
    else if (!fl_chr_is_digit(*tok->value))
    {
        tok->type = FL_TOKEN_TYPE_ID;
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

FL_INLINE_FN fl_bool _fl_token_is_wrapper(fl_token *tok, fl_char wrapper)
{
    return tok->value[0] == wrapper && tok->value[fl_str_len(tok->value) - 1] == wrapper;
}
