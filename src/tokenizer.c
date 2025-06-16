#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

static inline char* _tok_trim_left(const char* str, const char* delims) {
  if (str == NULL || delims == NULL) return (char*)str;

  while (*str && strchr(delims, *str)) str++;

  return (char*)str;
}

static char* _tok_make_escapes_first_char_view(const char** escapes,
                                               size_t escapes_num) {
  char* view = malloc(escapes_num + 1);
  for (size_t i = 0; i < escapes_num; i++) {
    view[i] = *escapes[i];
  }
  view[escapes_num] = '\0';
  return view;
}

static void _tok_free_escapes_first_char_view(char* view) { free(view); }

static char* _tok_next_token(char* str, const char** escapes,
                             size_t escapes_num, const char* delimiters) {
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

  next_pos = _tok_trim_left(next_pos, delimiters);
  if (*next_pos == '\0') return NULL;

  char* token_start = next_pos;

  size_t escape_len = 0;
  if (tok_is_escape_next(next_pos, escapes, escapes_num, &escape_len)) {
    next_pos += escape_len;
    escape_char = *next_pos;
    goto __return;
  }

  char* next_delim = strpbrk(next_pos, delimiters);
  char* next_escape = strpbrk(
      next_pos, _tok_make_escapes_first_char_view(escapes, escapes_num));

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

static inline size_t _tok_count_tokens(char* str, const char** escapes,
                                       size_t escapes_num, const char* delims) {
  size_t count = 0;
  if (_tok_next_token(str, escapes, escapes_num, delims))
    count++;
  else
    return 0;

  while (_tok_next_token(NULL, escapes, escapes_num, delims)) count++;

  return count;
}

char** tok_tokenize(char* str, const char** escapes, size_t escapes_num,
                    const char* delims, size_t* tokens_num) {
  // validate arguments
  if (str == NULL || escapes == NULL || escapes_num == 0 || delims == NULL ||
      tokens_num == NULL)
    return NULL;

  // count tokens
  char* str_copy = _strdup(str);
  size_t count = _tok_count_tokens(str_copy, escapes, escapes_num, delims);
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
  char* token = _tok_next_token(str_copy, escapes, escapes_num, delims);
  size_t test = 0;
  for (size_t index = 0; token; index++) {
    tokens[index] = _strdup(token);
    token = _tok_next_token(NULL, escapes, escapes_num, delims);
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

uint8_t tok_is_escape_next(const char* str, const char** escapes,
                           size_t escapes_num, size_t* escape_len) {
  if (str == NULL || escapes == NULL || escapes_num == 0) return 0;

  for (size_t i = 0; i < escapes_num; i++) {
    size_t len = strlen(escapes[i]);
    if (strncmp(str, escapes[i], len) == 0) {
      if (escape_len) *escape_len = len;
      return 1;
    }
  }

  return 0;
}
