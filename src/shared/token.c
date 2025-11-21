#include "shared/token.h"

#define _fl_token_is_in(what, tok_value) fl_str_in(tok_value, what, what##_num)

static fl_token_type _fl_token_deduce_type(fl_char *value, fl_tok_context *cnt);
static fl_token_alias _fl_token_deduce_alias(fl_char *value, fl_tok_context *cnt);
static fl_bool _fl_token_is_comment(fl_char *value, const fl_char *comment_wrapper);
static fl_bool _fl_token_is_literal(fl_char *value, const fl_char **literal_wrappers, fl_uint lw_num);

void fl_token_init(fl_token *token, fl_char *value, fl_tok_context *cnt)
{
    token->value = value;
    token->line = -1;
    token->column = -1;

    if (!cnt)
    {
        token->type = FL_TOKEN_TYPE_NONE;
        token->alias = FL_TOKEN_ALIAS_NONE;
    }
    else
    {
        token->type = _fl_token_deduce_type(value, cnt);
        token->alias = _fl_token_deduce_alias(value, cnt);
    }
}

static fl_token_type _fl_token_deduce_type(fl_char *value, fl_tok_context *cnt)
{
    if (!value)
        return FL_TOKEN_TYPE_NONE;

    if (_fl_token_is_in(cnt->delimeters, value))
        return FL_TOKEN_TYPE_DELIMITER;
    if (_fl_token_is_in(cnt->keywords, value))
        return FL_TOKEN_TYPE_KEYWORD;
    if (_fl_token_is_in(cnt->operators, value))
        return FL_TOKEN_TYPE_OPERATOR;
    if (_fl_token_is_comment(value, cnt->comment_wrapper))
        return FL_TOKEN_TYPE_COMMENT;
    if (_fl_token_is_literal(value, cnt->literal_wrappers, cnt->literal_wrappers_num))
        return FL_TOKEN_TYPE_LITERAL;
    else
        return FL_TOKEN_TYPE_IDENTIFIER;
}

static fl_bool _fl_token_is_literal(fl_char *value, const fl_char **literal_wrappers, fl_uint lw_num)
{
    for (fl_size i = 0; i < lw_num; i++)
    {
        const char *literal_wrapper = literal_wrappers[i];
        fl_size literal_token_len = fl_str_len(literal_wrapper);
        if (fl_str_equal_n(value, literal_wrapper, literal_token_len))
        {
            const char *literal_end_predicate = value + fl_str_len(value) - literal_token_len;

            if (fl_str_equal_n(literal_end_predicate, literal_wrapper, literal_token_len))
            {
                return FL_TRUE;
            }
        }
    }

    return FL_FALSE;
}

static fl_bool _fl_token_is_comment(fl_char *value, const fl_char *comment_wrapper)
{
    fl_size comment_token_len = fl_str_len(comment_wrapper);
    if (fl_str_equal_n(value, comment_wrapper, comment_token_len))
    {
        const char *comm_end_predicate = value + fl_str_len(value) - comment_token_len;

        if (fl_str_equal_n(comm_end_predicate, comment_wrapper, comment_token_len))
        {
            return FL_TRUE;
        }
    }
    return FL_FALSE;
}

static fl_token_alias _fl_token_deduce_alias(fl_char *value, fl_tok_context *cnt)
{
    if (!value || _fl_token_is_comment(value, cnt->comment_wrapper) ||
        _fl_token_is_literal(value, cnt->literal_wrappers, cnt->literal_wrappers_num))
    {
        return FL_TOKEN_ALIAS_NONE; // No alias for comments or literals
    }
    if (_fl_token_is_in(cnt->keywords, value))
    {
        if (fl_str_equal(value, "import"))
            return FL_TOKEN_ALIAS_IMPORT;
        if (fl_str_equal(value, "use"))
            return FL_TOKEN_ALIAS_EXPORT;
    }
    if (_fl_token_is_in(cnt->delimeters, value))
    {
        if (fl_str_equal(value, ","))
            return FL_TOKEN_ALIAS_COMMA;
        if (fl_str_equal(value, "("))
            return FL_TOKEN_ALIAS_LPAREN;
        if (fl_str_equal(value, ")"))
            return FL_TOKEN_ALIAS_RPAREN;
        if (fl_str_equal(value, "{"))
            return FL_TOKEN_ALIAS_LBRACE;
        if (fl_str_equal(value, "}"))
            return FL_TOKEN_ALIAS_RBRACE;
        if (fl_str_equal(value, ";"))
            return FL_TOKEN_ALIAS_SEMICOLON;
        if (fl_str_equal(value, "["))
            return FL_TOKEN_ALIAS_LBRACKET;
        if (fl_str_equal(value, "]"))
            return FL_TOKEN_ALIAS_RBRACKET;
        if (fl_str_equal(value, "."))
            return FL_TOKEN_ALIAS_DOT;
        if (fl_str_equal(value, "\n"))
            return FL_TOKEN_ALIAS_NL;
    }
    if (_fl_token_is_in(cnt->operators, value))
    {
        if (fl_str_equal(value, "+"))
            return FL_TOKEN_ALIAS_ADD;
        if (fl_str_equal(value, "-"))
            return FL_TOKEN_ALIAS_SUB;
        if (fl_str_equal(value, "*"))
            return FL_TOKEN_ALIAS_MUL;
        if (fl_str_equal(value, "/"))
            return FL_TOKEN_ALIAS_DIV;
        if (fl_str_equal(value, "%"))
            return FL_TOKEN_ALIAS_MOD;
        if (fl_str_equal(value, "&&"))
            return FL_TOKEN_ALIAS_AND;
        if (fl_str_equal(value, "||"))
            return FL_TOKEN_ALIAS_OR;
        if (fl_str_equal(value, "<"))
            return FL_TOKEN_ALIAS_LT;
        if (fl_str_equal(value, ">"))
            return FL_TOKEN_ALIAS_GT;
        if (fl_str_equal(value, "<="))
            return FL_TOKEN_ALIAS_LTE;
        if (fl_str_equal(value, ">="))
            return FL_TOKEN_ALIAS_GTE;
        if (fl_str_equal(value, "=="))
            return FL_TOKEN_ALIAS_EQ;
        if (fl_str_equal(value, "!="))
            return FL_TOKEN_ALIAS_NEQ;
        if (fl_str_equal(value, "="))
            return FL_TOKEN_ALIAS_ASSIGN;
        if (fl_str_equal(value, "!"))
            return FL_TOKEN_ALIAS_NOT;
        if (fl_str_equal(value, "&"))
            return FL_TOKEN_ALIAS_BIT_AND;
        if (fl_str_equal(value, "|"))
            return FL_TOKEN_ALIAS_BIT_OR;
        if (fl_str_equal(value, "~"))
            return FL_TOKEN_ALIAS_BIT_XOR;
        if (fl_str_equal(value, "<<"))
            return FL_TOKEN_ALIAS_BIT_LSHIFT;
        if (fl_str_equal(value, ">>"))
            return FL_TOKEN_ALIAS_BIT_RSHIFT;
        if (fl_str_equal(value, "?"))
            return FL_TOKEN_ALIAS_IF;
        if (fl_str_equal(value, ":"))
            return FL_TOKEN_ALIAS_COLON;
        if (fl_str_equal(value, "=>"))
            return FL_TOKEN_ALIAS_LAMBDA;
    }

    return FL_TOKEN_ALIAS_NONE; // Default alias for identifiers
}