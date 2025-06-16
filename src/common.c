#include "common.h"

#include "str.h"

const char *INP_TOKENIZER_DELIMS = " \r\t";

const char *INP_BLOCK_COMMENT = "###";

const char *INP_LINE_COMMENT = "#";

const char *INP_KEYWORDS[] = {"import"};

const char *INP_DELIMITERS[] = {",", "(", ")", "{",  "}",  ";",
                                "[", "]", ".", "\n", "\"", "'"};

const char *INP_OPERATORS[] = {
    "==", "=>", "!=", "<<", "<=", ">>", "<=", "<", ">", "=", "+", "-",
    "*",  "/",  "%",  "&&", "||", "!",  "&",  "|", "~", "?", ":",
};

size_t INP_KEYWORDS_NUM = inp_ssizeof(INP_KEYWORDS);
size_t INP_DELIMITERS_NUM = inp_ssizeof(INP_DELIMITERS);
size_t INP_OPERATORS_NUM = inp_ssizeof(INP_OPERATORS);

uint8_t inp_token_is_keyword(inp_token token) {
  return str_is_in(token.value, INP_KEYWORDS, inp_ssizeof(INP_KEYWORDS));
}
uint8_t inp_token_is_operator(inp_token token) {
  return str_is_in(token.value, INP_OPERATORS, inp_ssizeof(INP_KEYWORDS));
}

uint8_t inp_token_is_delimiter(inp_token token) {
  return str_is_in(token.value, INP_DELIMITERS, inp_ssizeof(INP_KEYWORDS));
}

uint8_t inp_token_is_block_comment(inp_token token) {
  return str_equal(token.value, INP_BLOCK_COMMENT);
}

uint8_t inp_token_is_line_comment(inp_token token) {
  return str_equal(token.value, INP_LINE_COMMENT);
}