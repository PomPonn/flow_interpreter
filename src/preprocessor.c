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

static void ppc_token_deduce_alias(inp_token *token) {
  if (token == NULL) return;

  if (inp_token_is_comment(*token) || inp_token_is_literal(*token)) {
    token->alias = COMP_TOKEN_ALIAS_NONE;  // No alias for comments or literals
    return;
  } else if (inp_token_is_keyword(*token)) {
    if (str_equal(token->value, "import")) {
      token->alias = COMP_TOKEN_ALIAS_IMPORT;
    } else if (str_equal(token->value, "export")) {
      token->alias = COMP_TOKEN_ALIAS_EXPORT;
    }
  } else if (inp_token_is_delimiter(*token)) {
    if (str_equal(token->value, ",")) {
      token->alias = COMP_TOKEN_ALIAS_COMMA;
    } else if (str_equal(token->value, "(")) {
      token->alias = COMP_TOKEN_ALIAS_LPAREN;
    } else if (str_equal(token->value, ")")) {
      token->alias = COMP_TOKEN_ALIAS_RPAREN;
    } else if (str_equal(token->value, "{")) {
      token->alias = COMP_TOKEN_ALIAS_LBRACE;
    } else if (str_equal(token->value, "}")) {
      token->alias = COMP_TOKEN_ALIAS_RBRACE;
    } else if (str_equal(token->value, ";")) {
      token->alias = COMP_TOKEN_ALIAS_SEMICOLON;
    } else if (str_equal(token->value, "[")) {
      token->alias = COMP_TOKEN_ALIAS_LBRACKET;
    } else if (str_equal(token->value, "]")) {
      token->alias = COMP_TOKEN_ALIAS_RBRACKET;
    } else if (str_equal(token->value, ".")) {
      token->alias = COMP_TOKEN_ALIAS_DOT;
    } else if (str_equal(token->value, "\n")) {
      token->alias = COMP_TOKEN_ALIAS_NL;
    }
  } else if (inp_token_is_operator(*token)) {
    if (str_equal(token->value, "+")) {
      token->alias = COMP_TOKEN_ALIAS_ADD;
    } else if (str_equal(token->value, "-")) {
      token->alias = COMP_TOKEN_ALIAS_SUB;
    } else if (str_equal(token->value, "*")) {
      token->alias = COMP_TOKEN_ALIAS_MUL;
    } else if (str_equal(token->value, "/")) {
      token->alias = COMP_TOKEN_ALIAS_DIV;
    } else if (str_equal(token->value, "%")) {
      token->alias = COMP_TOKEN_ALIAS_MOD;
    } else if (str_equal(token->value, "&&")) {
      token->alias = COMP_TOKEN_ALIAS_AND;
    } else if (str_equal(token->value, "||")) {
      token->alias = COMP_TOKEN_ALIAS_OR;
    } else if (str_equal(token->value, "<")) {
      token->alias = COMP_TOKEN_ALIAS_LT;
    } else if (str_equal(token->value, ">")) {
      token->alias = COMP_TOKEN_ALIAS_GT;
    } else if (str_equal(token->value, "<=")) {
      token->alias = COMP_TOKEN_ALIAS_LTE;
    } else if (str_equal(token->value, ">=")) {
      token->alias = COMP_TOKEN_ALIAS_GTE;
    } else if (str_equal(token->value, "==")) {
      token->alias = COMP_TOKEN_ALIAS_EQ;
    } else if (str_equal(token->value, "!=")) {
      token->alias = COMP_TOKEN_ALIAS_NEQ;
    } else if (str_equal(token->value, "=")) {
      token->alias = COMP_TOKEN_ALIAS_ASSIGN;
    } else if (str_equal(token->value, "!")) {
      token->alias = COMP_TOKEN_ALIAS_NOT;
    } else if (str_equal(token->value, "&")) {
      token->alias = COMP_TOKEN_ALIAS_BIT_AND;
    } else if (str_equal(token->value, "|")) {
      token->alias = COMP_TOKEN_ALIAS_BIT_OR;
    } else if (str_equal(token->value, "~")) {
      token->alias = COMP_TOKEN_ALIAS_BIT_XOR;
    } else if (str_equal(token->value, "<<")) {
      token->alias = COMP_TOKEN_ALIAS_BIT_LSHIFT;
    } else if (str_equal(token->value, ">>")) {
      token->alias = COMP_TOKEN_ALIAS_BIT_RSHIFT;
    } else if (str_equal(token->value, "?")) {
      token->alias = COMP_TOKEN_ALIAS_IF;
    } else if (str_equal(token->value, ":")) {
      token->alias = COMP_TOKEN_ALIAS_ELSE;
    } else if (str_equal(token->value, "=>")) {
      token->alias = COMP_TOKEN_ALIAS_LAMBDA;
    }
  } else {
    token->alias = COMP_TOKEN_ALIAS_NONE;  // Default alias for identifiers
  }
}

void ppc_preprocess(inp_token *tokens, size_t tokens_num) {
  for (size_t i = 0; i < tokens_num; i++) {
    if (tokens[i].value == NULL) continue;
    ppc_token_deduce_type(&tokens[i]);
    ppc_token_deduce_alias(&tokens[i]);
  }
}