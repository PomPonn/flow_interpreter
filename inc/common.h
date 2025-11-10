#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdint.h>

#define inp_ssizeof(x) (sizeof(x) / sizeof((x)[0]))

extern const char *INP_TOKENIZER_DELIMS;

extern const char *INP_COMMENT_TOKEN;

extern const char *INP_KEYWORDS[];
extern size_t INP_KEYWORDS_NUM;

extern const char *INP_DELIMITERS[];
extern size_t INP_DELIMITERS_NUM;

extern const char *INP_OPERATORS[];
extern size_t INP_OPERATORS_NUM;

extern const char *INP_LITERAL_WRAPPERS[];
extern size_t INP_LITERAL_WRAPPERS_NUM;

enum _inp_token_type
{
  COMP_TOKEN_TYPE_UNKNOWN = 0,
  COMP_TOKEN_TYPE_KEYWORD,
  COMP_TOKEN_TYPE_IDENTIFIER,
  COMP_TOKEN_TYPE_LITERAL,
  COMP_TOKEN_TYPE_OPERATOR,
  COMP_TOKEN_TYPE_DELIMITER,
  COMP_TOKEN_TYPE_COMMENT
};
typedef enum _inp_token_type inp_token_type;

enum _inp_token_alias
{
  COMP_TOKEN_ALIAS_NONE = 0,
  // keywords
  COMP_TOKEN_ALIAS_IMPORT,
  COMP_TOKEN_ALIAS_EXPORT,
  // delimiters
  COMP_TOKEN_ALIAS_COMMA,
  COMP_TOKEN_ALIAS_LPAREN,    // (
  COMP_TOKEN_ALIAS_RPAREN,    // )
  COMP_TOKEN_ALIAS_LBRACE,    // {
  COMP_TOKEN_ALIAS_RBRACE,    // }
  COMP_TOKEN_ALIAS_SEMICOLON, // ;
  COMP_TOKEN_ALIAS_LBRACKET,  // [
  COMP_TOKEN_ALIAS_RBRACKET,  // ]
  COMP_TOKEN_ALIAS_DOT,       // .
  COMP_TOKEN_ALIAS_NL,        // \n
  // operators
  COMP_TOKEN_ALIAS_ADD,        // +
  COMP_TOKEN_ALIAS_SUB,        // -
  COMP_TOKEN_ALIAS_MUL,        // *
  COMP_TOKEN_ALIAS_DIV,        // /
  COMP_TOKEN_ALIAS_MOD,        // %
  COMP_TOKEN_ALIAS_AND,        // &&
  COMP_TOKEN_ALIAS_OR,         // ||
  COMP_TOKEN_ALIAS_NOT,        // !
  COMP_TOKEN_ALIAS_BIT_AND,    // &
  COMP_TOKEN_ALIAS_BIT_OR,     // |
  COMP_TOKEN_ALIAS_BIT_XOR,    // ~
  COMP_TOKEN_ALIAS_BIT_LSHIFT, // <<
  COMP_TOKEN_ALIAS_BIT_RSHIFT, // >>
  COMP_TOKEN_ALIAS_EQ,         // ==
  COMP_TOKEN_ALIAS_NEQ,        // !=
  COMP_TOKEN_ALIAS_LT,         // <
  COMP_TOKEN_ALIAS_GT,         // >
  COMP_TOKEN_ALIAS_LTE,        // <=
  COMP_TOKEN_ALIAS_GTE,        // >=
  COMP_TOKEN_ALIAS_ASSIGN,     // =
  COMP_TOKEN_ALIAS_IF,         // ?
  COMP_TOKEN_ALIAS_COLON,      // :
  COMP_TOKEN_ALIAS_LAMBDA,     // =>
};
typedef enum _inp_token_alias inp_token_alias;

struct _inp_token
{
  inp_token_type type;
  inp_token_alias alias;
  char *value;
  size_t line;
  size_t column;
};
typedef struct _inp_token inp_token;

uint8_t inp_token_is_keyword(inp_token token);

uint8_t inp_token_is_operator(inp_token token);

uint8_t inp_token_is_delimiter(inp_token token);

uint8_t inp_token_is_comment(inp_token token);

uint8_t inp_token_is_literal(inp_token token);