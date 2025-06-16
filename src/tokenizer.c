#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

static char* _tok_trim_left(const char* str, const char* delims) {
  if (str == NULL || delims == NULL) return (char*)str;

  while (*str && strchr(delims, *str)) str++;

  return (char*)str;
}

static char* _tok_make_escapes_peek(tok_context cnt) {
  char* view = malloc(cnt.escapes_num + cnt.literal_wrappers_num + 1);

  for (size_t i = 0; i < cnt.escapes_num; i++) {
    view[i] = *cnt.escapes[i];
  }

  for (size_t i = 0; i < cnt.literal_wrappers_num; i++) {
    view[cnt.escapes_num + i] = *cnt.literal_wrappers[i];
  }

  view[cnt.escapes_num] = '\0';
  return view;
}

static void _tok_free_escapes_peek(char* view) { free(view); }

static uint8_t _tok_escape_next(const char* str, tok_context cnt,
                                size_t* escape_len) {
  for (size_t i = 0; i < cnt.escapes_num; i++) {
    size_t len = strlen(cnt.escapes[i]);
    if (strncmp(str, cnt.escapes[i], len) == 0) {
      if (escape_len) *escape_len = len;
      return 1;
    }
  }

  return 0;
}

static uint8_t _tok_literal_wrapper_next(const char* str, tok_context cnt,
                                         size_t* literal_len) {
  for (size_t i = 0; i < cnt.literal_wrappers_num; i++) {
    size_t len = strlen(cnt.literal_wrappers[i]);
    if (strncmp(str, cnt.literal_wrappers[i], len) == 0) {
      const char* ptr = str + len;
      while (*ptr && strncmp(ptr, cnt.literal_wrappers[i], len) != 0) ptr++;
      if (*ptr) ptr++;
      *literal_len = ptr - str;
      return 1;
    }
  }

  return 0;
}

static uint8_t _tok_comment_wrapper_next(const char* str, tok_context cnt,
                                         size_t* comment_len) {
  size_t len = strlen(cnt.comment_wrapper);
  if (strncmp(str, cnt.comment_wrapper, len) == 0) {
    const char* ptr = str + len;
    while (*ptr && strncmp(ptr, cnt.comment_wrapper, len) != 0) ptr++;
    if (*ptr) ptr++;
    *comment_len = ptr - str;
    return 1;
  }

  return 0;
}

static char* _tok_next_token(char* str, tok_context cnt) {
  static char* next_pos = NULL;
  static char escape_char = '\0';

  if (str) {
    next_pos = str;
    escape_char = '\0';
  } else if (next_pos == NULL)
    return NULL;

  if (escape_char) {
    next_pos--;
    *next_pos = escape_char;
    escape_char = '\0';
  }

  next_pos = _tok_trim_left(next_pos, cnt.delims);
  if (*next_pos == '\0') return NULL;

  char* token_start = next_pos;

  // Check for literal wrappers
  size_t literal_len = 0;
  if (_tok_literal_wrapper_next(next_pos, cnt, &literal_len)) {
    next_pos += literal_len;
    escape_char = *next_pos;

    goto __return;
  }

  // Check for comment wrapper
  size_t comment_len = 0;
  if (_tok_comment_wrapper_next(next_pos, cnt, &comment_len)) {
    next_pos += comment_len;
    escape_char = *next_pos;

    goto __return;
  }

  size_t escape_len = 0;
  if (_tok_escape_next(next_pos, cnt, &escape_len)) {
    next_pos += escape_len;
    escape_char = *next_pos;
    goto __return;
  }

  char* next_delim = strpbrk(next_pos, cnt.delims);
  char* next_escape = strpbrk(next_pos, cnt.escapes_peek);

  if (next_escape == NULL && next_delim == NULL) {
    next_pos = NULL;
    return token_start;
  }

  next_pos = min(next_escape, next_delim);

  if (next_escape == next_pos) {
    escape_char = *next_pos;
    goto __return;
  }

__return:
  if (*next_pos == '\0') {
    next_pos = NULL;
  } else {
    *next_pos = '\0';
    next_pos++;
  }

  return token_start;
}

static size_t _tok_count_tokens(char* str, tok_context cnt) {
  size_t count = 0;
  if (_tok_next_token(str, cnt))
    count++;
  else
    return 0;

  while (_tok_next_token(NULL, cnt)) count++;

  return count;
}

tok_context tok_make_context() {
  tok_context context;
  context.delims = INP_TOKENIZER_DELIMS;

  context.literal_wrappers = INP_LITERAL_WRAPPERS;
  context.literal_wrappers_num = INP_LITERAL_WRAPPERS_NUM;

  context.comment_wrapper = INP_COMMENT_TOKEN;

  size_t escapes_num = 1;  // for comment wrapper token
  escapes_num += INP_DELIMITERS_NUM + INP_OPERATORS_NUM;
  context.escapes_num = escapes_num;
  context.escapes = malloc(escapes_num * sizeof(char*));

  // fill escapes
  size_t i = 0;
  context.escapes[i++] = INP_COMMENT_TOKEN;
  for (size_t j = 0; j < INP_DELIMITERS_NUM; j++) {
    context.escapes[i++] = INP_DELIMITERS[j];
  }
  for (size_t j = 0; j < INP_OPERATORS_NUM; j++) {
    context.escapes[i++] = INP_OPERATORS[j];
  }

  context.escapes_peek = _tok_make_escapes_peek(context);

  return context;
}

void tok_free_context(tok_context* context) {
  if (context == NULL) return;

  _tok_free_escapes_peek(context->escapes_peek);
  free(context->escapes);
  context->escapes = NULL;
  context->escapes_num = 0;
  context->escapes_peek = NULL;
  context->literal_wrappers = NULL;
  context->comment_wrapper = NULL;
  context->literal_wrappers_num = 0;
  context->delims = NULL;
}

char** tok_tokenize(char* str, tok_context context, size_t* tokens_num) {
  // validate arguments
  if (str == NULL, tokens_num == NULL) return NULL;

  // count tokens
  char* str_copy = _strdup(str);
  size_t count = _tok_count_tokens(str_copy, context);
  free(str_copy);

  if (count == 0) {
    *tokens_num = 0;
    return NULL;
  }

  // Allocate memory for tokens
  char** tokens = calloc(count, sizeof(char*));
  if (tokens == NULL) return NULL;

  str_copy = _strdup(str);

  // Tokenize the string
  char* token = _tok_next_token(str_copy, context);
  size_t test = 0;
  for (size_t index = 0; token; index++) {
    tokens[index] = _strdup(token);
    token = _tok_next_token(NULL, context);
    test = index;
  }

  free(str_copy);

  *tokens_num = count;
  return tokens;
}

void tok_free_tokens(char** tokens, size_t tokens_num) {
  if (tokens == NULL) return;

  for (size_t i = 0; i < tokens_num; i++) free(tokens[i]);
  free(tokens);
}
