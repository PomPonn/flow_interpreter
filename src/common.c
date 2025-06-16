#include "common.h"

#include <string.h>

#include "str.h"

const char *INP_TOKENIZER_DELIMS = " \r\t";

const char *INP_COMMENT_TOKEN = "#";

const char *INP_KEYWORDS[] = {"import"};

const char *INP_DELIMITERS[] = {",", "(", ")", "{", "}",
                                ";", "[", "]", ".", "\n"};

const char *INP_OPERATORS[] = {
    "==", "=>", "!=", "<<", "<=", ">>", "<=", "<", ">", "=", "+", "-",
    "*",  "/",  "%",  "&&", "||", "!",  "&",  "|", "~", "?", ":",
};

const char *INP_LITERAL_WRAPPERS[] = {"'", "\""};

size_t INP_KEYWORDS_NUM = inp_ssizeof(INP_KEYWORDS);
size_t INP_DELIMITERS_NUM = inp_ssizeof(INP_DELIMITERS);
size_t INP_OPERATORS_NUM = inp_ssizeof(INP_OPERATORS);
size_t INP_LITERAL_WRAPPERS_NUM = inp_ssizeof(INP_LITERAL_WRAPPERS);

uint8_t inp_token_is_keyword(inp_token token) {
  return str_is_in(token.value, INP_KEYWORDS, inp_ssizeof(INP_KEYWORDS));
}
uint8_t inp_token_is_operator(inp_token token) {
  return str_is_in(token.value, INP_OPERATORS, inp_ssizeof(INP_OPERATORS));
}

uint8_t inp_token_is_delimiter(inp_token token) {
  return str_is_in(token.value, INP_DELIMITERS, inp_ssizeof(INP_DELIMITERS));
}

uint8_t inp_token_is_comment(inp_token token) {
  size_t comment_token_len = strlen(INP_COMMENT_TOKEN);
  if (strncmp(token.value, INP_COMMENT_TOKEN, comment_token_len) == 0) {
    const char *comm_end_predicate =
        token.value + strlen(token.value) - comment_token_len;

    if (strncmp(comm_end_predicate, INP_COMMENT_TOKEN, comment_token_len) ==
        0) {
      return 1;  // It's a comment token
    }
  }
  return 0;
}

uint8_t inp_token_is_literal(inp_token token) {
  for (size_t i = 0; i < INP_LITERAL_WRAPPERS_NUM; i++) {
    const char *literal_wrapper = INP_LITERAL_WRAPPERS[i];
    size_t literal_token_len = strlen(literal_wrapper);
    if (strncmp(token.value, literal_wrapper, literal_token_len) == 0) {
      const char *literal_end_predicate =
          token.value + strlen(token.value) - literal_token_len;

      if (strncmp(literal_end_predicate, literal_wrapper, literal_token_len) ==
          0) {
        return 1;  // It's a literal token
      }
    }
  }

  return 0;
}