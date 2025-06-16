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

enum _inp_token_type {
  COMP_TOKEN_TYPE_UNKNOWN,
  COMP_TOKEN_TYPE_KEYWORD,
  COMP_TOKEN_TYPE_IDENTIFIER,
  COMP_TOKEN_TYPE_LITERAL,
  COMP_TOKEN_TYPE_OPERATOR,
  COMP_TOKEN_TYPE_DELIMITER,
  COMP_TOKEN_TYPE_COMMENT
};
typedef enum _inp_token_type inp_token_type;

struct _inp_token {
  inp_token_type type;
  const char *value;
};
typedef struct _inp_token inp_token;

uint8_t inp_token_is_keyword(inp_token token);

uint8_t inp_token_is_operator(inp_token token);

uint8_t inp_token_is_delimiter(inp_token token);

uint8_t inp_token_is_comment(inp_token token);

uint8_t inp_token_is_literal(inp_token token);