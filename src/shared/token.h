#pragma once

#include "shared/common.h"
#include "shared/str.h"
#include "tokenizer/context.h"

enum _fl_token_type
{
    FL_TOKEN_TYPE_NONE,
    FL_TOKEN_TYPE_KEYWORD,
    FL_TOKEN_TYPE_IDENTIFIER,
    FL_TOKEN_TYPE_LITERAL,
    FL_TOKEN_TYPE_OPERATOR,
    FL_TOKEN_TYPE_DELIMITER,
    FL_TOKEN_TYPE_COMMENT
};
typedef fl_ubyte fl_token_type;

enum _fl_token_alias
{
    FL_TOKEN_ALIAS_NONE,
    // keywords
    FL_TOKEN_ALIAS_USE,
    FL_TOKEN_ALIAS_EXPORT,
    // delimiters
    FL_TOKEN_ALIAS_COMMA,
    FL_TOKEN_ALIAS_LPAREN,    // (
    FL_TOKEN_ALIAS_RPAREN,    // )
    FL_TOKEN_ALIAS_LBRACE,    // {
    FL_TOKEN_ALIAS_RBRACE,    // }
    FL_TOKEN_ALIAS_SEMICOLON, // ;
    FL_TOKEN_ALIAS_LBRACKET,  // [
    FL_TOKEN_ALIAS_RBRACKET,  // ]
    FL_TOKEN_ALIAS_DOT,       // .
    FL_TOKEN_ALIAS_NL,        // \n
    // operators
    FL_TOKEN_ALIAS_ADD,        // +
    FL_TOKEN_ALIAS_SUB,        // -
    FL_TOKEN_ALIAS_MUL,        // *
    FL_TOKEN_ALIAS_DIV,        // /
    FL_TOKEN_ALIAS_MOD,        // %
    FL_TOKEN_ALIAS_AND,        // &&
    FL_TOKEN_ALIAS_OR,         // ||
    FL_TOKEN_ALIAS_NOT,        // !
    FL_TOKEN_ALIAS_BIT_AND,    // &
    FL_TOKEN_ALIAS_BIT_OR,     // |
    FL_TOKEN_ALIAS_BIT_XOR,    // ~
    FL_TOKEN_ALIAS_BIT_LSHIFT, // <<
    FL_TOKEN_ALIAS_BIT_RSHIFT, // >>
    FL_TOKEN_ALIAS_EQ,         // ==
    FL_TOKEN_ALIAS_NEQ,        // !=
    FL_TOKEN_ALIAS_LT,         // <
    FL_TOKEN_ALIAS_GT,         // >
    FL_TOKEN_ALIAS_LTE,        // <=
    FL_TOKEN_ALIAS_GTE,        // >=
    FL_TOKEN_ALIAS_ASSIGN,     // =
    FL_TOKEN_ALIAS_IF,         // ?
    FL_TOKEN_ALIAS_COLON,      // :
    FL_TOKEN_ALIAS_LAMBDA,     // =>
};
typedef fl_ubyte fl_token_alias;

struct _fl_token
{
    fl_token_type type;
    fl_token_alias alias;
    fl_char *value;
    fl_size line;
    fl_size column;
};
typedef struct _fl_token fl_token;

void fl_token_init(fl_token *token, fl_char *value, fl_tok_context *context);

fl_char *fl_token_str_as_printable(fl_char *token_str);