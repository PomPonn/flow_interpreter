#pragma once

#include "shared/common.h"
#include "shared/str.h"

enum _fl_token_type
{
    FL_TOKEN_TYPE_NONE,
    FL_TOKEN_TYPE_ID,
    FL_TOKEN_TYPE_LITERAL,
    FL_TOKEN_TYPE_DELIMITER,
    FL_TOKEN_TYPE_COMMENT
};
typedef fl_byte fl_token_type;

enum _fl_token_alias
{
    // delimiters
    // the order reflects indexes in
    // FL_ORDERED_DELIMS declared in shared/delims.h
    FL_TOKEN_ALIAS_NONE,
    FL_TOKEN_ALIAS_SEMICOLON, // ;
    FL_TOKEN_ALIAS_DOT,       // .
    FL_TOKEN_ALIAS_COMMA,     // ,
    FL_TOKEN_ALIAS_LPAREN,    // (
    FL_TOKEN_ALIAS_RPAREN,    // )
    FL_TOKEN_ALIAS_LBRACE,    // {
    FL_TOKEN_ALIAS_RBRACE,    // }
    FL_TOKEN_ALIAS_LBRACKET,  // [
    FL_TOKEN_ALIAS_RBRACKET,  // ]
                              // literals
    FL_TOKEN_ALIAS_STRING,
    FL_TOKEN_ALIAS_NUMBER,
};
typedef fl_byte fl_token_alias;

struct _fl_token_location
{
    // e.g. filename
    const fl_char *description;
    fl_size line;
    fl_size column;
};
typedef struct _fl_token_location fl_token_location;

struct _fl_token
{
    fl_token_type type;
    fl_token_alias alias;
    fl_char *value;
    fl_token_location location;
};
typedef struct _fl_token fl_token;

fl_bool fl_token_init(fl_token *token, fl_char *value, fl_token_location location);

fl_void fl_token_free(fl_token *token);

fl_char *fl_token_as_printable(fl_char *token_str);

fl_bool fl_token_is_delim(fl_token *token);