#include "compiler.h"

#include <string.h>

#include "str.h"

const char *COMP_KEYWORDS[] = {"import"};
const char *COMP_OPERATORS[] = {
    "=",
    "==",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
    "+",
    "-",
    "*",
    "/",
    "%",
    "&&",
    "||",
    "!",
    "&",
    "|"
    "~",
    "<<",
    ">>",
    "?",
    ":",
};

enum _comp_token_type {
  COMP_TOKEN_TYPE_UNKNOWN,
  COMP_TOKEN_TYPE_KEYWORD,
  COMP_TOKEN_TYPE_IDENTIFIER,
  COMP_TOKEN_TYPE_LITERAL,
  COMP_TOKEN_TYPE_OPERATOR,
  COMP_TOKEN_TYPE_DELIMITER,
  COMP_TOKEN_TYPE_COMMENT
};
typedef enum _comp_token_type comp_token_type;

struct _comp_token_info {
  comp_token_type type;
  const char *value;
  size_t length;
};
typedef struct _comp_token_info comp_token_info;

static inline void _comp_is_operator(const char *tokens, size_t tokens_count,
                                     size_t token_index,
                                     comp_token_info *token_info) {
  for (const char *op_ptr = COMP_OPERATORS; *op_ptr; op_ptr++) {
    for (const char *c_ptr = op_ptr; *c_ptr; c_ptr++) {
      if (tokens[token_index] != *c_ptr) {
        break;
      }
    }

    if (tokens[token_index] == *op_ptr) {
    }
  }
}
static void _comp_code_tokens(char **tokens, size_t tokens_num) {
  for (size_t i = 0; i < tokens_num; i++) {
    if (tokens[i] == NULL) continue;

    comp_token_info token_info = {COMP_TOKEN_TYPE_UNKNOWN, tokens[i],
                                  strlen(tokens[i])};

    if (token_info.length) {
    }
  }
}

void comp_compile_to_bytecode(char **tokens, size_t tokens_num) {}