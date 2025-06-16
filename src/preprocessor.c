#include "preprocessor.h"

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

void ppc_preprocess(char **tokens, size_t tokens_num) {
  for (size_t i = 0; i < tokens_num; i++) {
    if (tokens[i] == NULL) continue;

    inp_token token = {COMP_TOKEN_TYPE_UNKNOWN, tokens[i]};

    ppc_token_deduce_type(&token);
  }
}