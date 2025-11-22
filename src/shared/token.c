#include "shared/token.h"

#include "stdio.h"

// --- DEFINES ---

#define _fl_token_is_in(what, tok) fl_str_in(tok->value, what, what##_num)

// --- STATIC FUNCTIONS DEFINITION ---

static fl_bool _fl_token_deduce(fl_token *tok, fl_tok_context *cnt);
static fl_bool _fl_token_is_comment(fl_token *token, const fl_char *comment_wrapper);
static fl_bool _fl_token_is_string_literal(fl_token *token, const fl_char **string_wrappers, fl_uint sw_num);
static fl_bool _fl_token_is_number_literal(fl_token *token);
static fl_bool _fl_token_is_valid_identifier(fl_token *token);

// --- IMPLEMENTATIONS ---

fl_char *fl_token_str_as_printable(fl_char *tok)
{
    if (*tok == '\n')
    {
        return "\\n";
    }
    else
    {
        return tok;
    }
}

fl_bool fl_token_init(fl_token *token, fl_char *value, fl_tok_context *cnt)
{
    token->value = value;
    token->line = 0;
    token->column = 0;

    if (!cnt)
    {
        token->type = FL_TOKEN_TYPE_NONE;
        token->alias = FL_TOKEN_ALIAS_NONE;

        return FL_TRUE;
    }
    else
    {
        return _fl_token_deduce(token, cnt);
    }
}

static fl_bool _fl_token_deduce(fl_token *tok, fl_tok_context *cnt)
{
    if (!tok || !cnt)
        return FL_FALSE;

    if (_fl_token_is_in(cnt->delimeters, tok))
    {
        tok->type = FL_TOKEN_TYPE_DELIMITER;

        if (fl_str_equal(tok->value, ","))
            tok->alias = FL_TOKEN_ALIAS_COMMA;
        if (fl_str_equal(tok->value, "("))
            tok->alias = FL_TOKEN_ALIAS_LPAREN;
        if (fl_str_equal(tok->value, ")"))
            tok->alias = FL_TOKEN_ALIAS_RPAREN;
        if (fl_str_equal(tok->value, "{"))
            tok->alias = FL_TOKEN_ALIAS_LBRACE;
        if (fl_str_equal(tok->value, "}"))
            tok->alias = FL_TOKEN_ALIAS_RBRACE;
        if (fl_str_equal(tok->value, ";"))
            tok->alias = FL_TOKEN_ALIAS_SEMICOLON;
        if (fl_str_equal(tok->value, "["))
            tok->alias = FL_TOKEN_ALIAS_LBRACKET;
        if (fl_str_equal(tok->value, "]"))
            tok->alias = FL_TOKEN_ALIAS_RBRACKET;
        if (fl_str_equal(tok->value, "."))
            tok->alias = FL_TOKEN_ALIAS_DOT;
        if (fl_str_equal(tok->value, "\n"))
            tok->alias = FL_TOKEN_ALIAS_NL;
    }
    else if (_fl_token_is_in(cnt->operators, tok))
    {
        tok->type = FL_TOKEN_TYPE_OPERATOR;

        if (fl_str_equal(tok->value, "+"))
            tok->alias = FL_TOKEN_ALIAS_ADD;
        if (fl_str_equal(tok->value, "-"))
            tok->alias = FL_TOKEN_ALIAS_SUB;
        if (fl_str_equal(tok->value, "*"))
            tok->alias = FL_TOKEN_ALIAS_MUL;
        if (fl_str_equal(tok->value, "/"))
            tok->alias = FL_TOKEN_ALIAS_DIV;
        if (fl_str_equal(tok->value, "%"))
            tok->alias = FL_TOKEN_ALIAS_MOD;
        if (fl_str_equal(tok->value, "&&"))
            tok->alias = FL_TOKEN_ALIAS_AND;
        if (fl_str_equal(tok->value, "||"))
            tok->alias = FL_TOKEN_ALIAS_OR;
        if (fl_str_equal(tok->value, "<"))
            tok->alias = FL_TOKEN_ALIAS_LT;
        if (fl_str_equal(tok->value, ">"))
            tok->alias = FL_TOKEN_ALIAS_GT;
        if (fl_str_equal(tok->value, "<="))
            tok->alias = FL_TOKEN_ALIAS_LTE;
        if (fl_str_equal(tok->value, ">="))
            tok->alias = FL_TOKEN_ALIAS_GTE;
        if (fl_str_equal(tok->value, "=="))
            tok->alias = FL_TOKEN_ALIAS_EQ;
        if (fl_str_equal(tok->value, "!="))
            tok->alias = FL_TOKEN_ALIAS_NEQ;
        if (fl_str_equal(tok->value, "="))
            tok->alias = FL_TOKEN_ALIAS_ASSIGN;
        if (fl_str_equal(tok->value, "!"))
            tok->alias = FL_TOKEN_ALIAS_NOT;
        if (fl_str_equal(tok->value, "&"))
            tok->alias = FL_TOKEN_ALIAS_BIT_AND;
        if (fl_str_equal(tok->value, "|"))
            tok->alias = FL_TOKEN_ALIAS_BIT_OR;
        if (fl_str_equal(tok->value, "~"))
            tok->alias = FL_TOKEN_ALIAS_BIT_XOR;
        if (fl_str_equal(tok->value, "<<"))
            tok->alias = FL_TOKEN_ALIAS_BIT_LSHIFT;
        if (fl_str_equal(tok->value, ">>"))
            tok->alias = FL_TOKEN_ALIAS_BIT_RSHIFT;
        if (fl_str_equal(tok->value, "?"))
            tok->alias = FL_TOKEN_ALIAS_IF;
        if (fl_str_equal(tok->value, ":"))
            tok->alias = FL_TOKEN_ALIAS_COLON;
        if (fl_str_equal(tok->value, "=>"))
            tok->alias = FL_TOKEN_ALIAS_LAMBDA;
    }
    else if (_fl_token_is_in(cnt->keywords, tok))
    {
        tok->type = FL_TOKEN_TYPE_KEYWORD;

        if (fl_str_equal(tok->value, "use"))
            tok->alias = FL_TOKEN_ALIAS_USE;
        if (fl_str_equal(tok->value, "export"))
            tok->alias = FL_TOKEN_ALIAS_EXPORT;
    }
    else if (_fl_token_is_number_literal(tok))
    {
        tok->type = FL_TOKEN_TYPE_LITERAL;
        tok->alias = FL_TOKEN_ALIAS_NUMBER;
    }
    else if (_fl_token_is_comment(tok, cnt->comment_wrapper))
    {
        tok->type = FL_TOKEN_TYPE_COMMENT;
        tok->alias = FL_TOKEN_ALIAS_NONE;
    }
    else if (_fl_token_is_string_literal(tok, cnt->string_wrappers, cnt->string_wrappers_num))
    {
        tok->type = FL_TOKEN_TYPE_LITERAL;
        tok->alias = FL_TOKEN_ALIAS_STRING;
    }
    else if (_fl_token_is_valid_identifier(tok))
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

static fl_bool _fl_token_is_string_literal(fl_token *token, const fl_char **string_wrappers, fl_uint sw_num)
{
    for (fl_size i = 0; i < sw_num; i++)
    {
        const char *literal_wrapper = string_wrappers[i];
        fl_size literal_token_len = fl_str_len(literal_wrapper);
        if (fl_str_equal_n(token->value, literal_wrapper, literal_token_len))
        {
            const char *literal_end_predicate = token->value + fl_str_len(token->value) - literal_token_len;

            if (fl_str_equal_n(literal_end_predicate, literal_wrapper, literal_token_len))
            {
                return FL_TRUE;
            }
        }
    }

    return FL_FALSE;
}

static fl_bool _fl_token_is_number_literal(fl_token *token)
{
    return fl_str_is_num(token->value);
}

static fl_bool _fl_token_is_valid_identifier(fl_token *token)
{
    return !fl_chr_is_digit(*token->value);
}

static fl_bool _fl_token_is_comment(fl_token *tok, const fl_char *comment_wrapper)
{
    fl_size comment_token_len = fl_str_len(comment_wrapper);
    if (fl_str_equal_n(tok->value, comment_wrapper, comment_token_len))
    {
        const char *comm_end_predicate = tok->value + fl_str_len(tok->value) - comment_token_len;

        if (fl_str_equal_n(comm_end_predicate, comment_wrapper, comment_token_len))
        {
            return FL_TRUE;
        }
    }
    return FL_FALSE;
}