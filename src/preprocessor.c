#include "preprocessor.h"

#include <stdlib.h>
#include <string.h>

#include "str.h"

static void ppc_token_deduce_type(inp_token *token) {
  if (token == NULL) return;

  if (inp_token_is_delimiter(*token)) {
    token->type = COMP_TOKEN_TYPE_DELIMITER;
  } else if (inp_token_is_keyword(*token)) {
    token->type = COMP_TOKEN_TYPE_KEYWORD;
  } else if (inp_token_is_operator(*token)) {
    token->type = COMP_TOKEN_TYPE_OPERATOR;
  } else if (inp_token_is_comment(*token)) {
    token->type = COMP_TOKEN_TYPE_COMMENT;
  } else if (inp_token_is_literal(*token)) {
    token->type = COMP_TOKEN_TYPE_LITERAL;
  } else {
    token->type = COMP_TOKEN_TYPE_IDENTIFIER;
  }
}

inp_token *ppc_preprocess(char **tokens, size_t tokens_num) {
  inp_token *processed_tokens = malloc(tokens_num * sizeof(inp_token));

  for (size_t i = 0; i < tokens_num; i++) {
    if (tokens[i] == NULL) continue;

    processed_tokens[i].type = COMP_TOKEN_TYPE_UNKNOWN;
    processed_tokens[i].value = tokens[i];

    ppc_token_deduce_type(&processed_tokens[i]);
  }

  return processed_tokens;
}