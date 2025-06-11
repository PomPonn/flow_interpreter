#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

static inline const char* _tok_trim_left(const char* str, const char* delims) {
  if (str == NULL || delims == NULL) return str;

  while (*str && strchr(delims, *str)) str++;

  return str;
}

static inline const char* _tok_next_stop(const char* str, const char* escapes,
                                         const char* delims) {
  if (str == NULL || escapes == NULL || delims == NULL) return NULL;

  static const char* last_stop = NULL;

  if (str) {
    last_stop = str;
  } else if (last_stop == NULL) {
    return NULL;
  }

  last_stop = _tok_trim_left(last_stop, delims);

  char* next_delim = strpbrk(last_stop, delims);
  char* next_escape = strpbrk(last_stop, escapes);

  if (next_escape == NULL && next_delim == NULL) return NULL;

  last_stop = min(next_escape, next_delim);

  const char* res = last_stop;
  if (next_escape == last_stop) {
    last_stop++;
  }

  return res;
}

static inline size_t _tok_count_tokens(const char* str, const char* escapes,
                                       const char* delims) {
  if (str == NULL || escapes == NULL || delims == NULL) return 0;

  size_t count = 0;
  const char* ptr = str;
  while (ptr = _tok_next_stop(ptr, escapes, delims)) {
    count++;
  }

  return count;
}

static char* _tok_next_token(char* str, const char* escapes,
                             const char* delimiters) {
  if (escapes == NULL || delimiters == NULL) return NULL;

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

  if (strchr(escapes, *next_pos)) {
    next_pos++;
    escape_char = *next_pos;
    goto __return;
  }

  char* next_delim = strpbrk(next_pos, delimiters);
  char* next_escape = strpbrk(next_pos, escapes);

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

char** tok_tokenize(const char* str, const char* escapes, const char* delims,
                    size_t* tokens_num) {
  // validate arguments
  if (str == NULL || escapes == NULL || delims == NULL || tokens_num == NULL)
    return NULL;

  // count tokens
  size_t count = _tok_count_tokens(str, escapes, delims);

  if (count == 0) {
    *tokens_num = 0;
    return NULL;
  }

  // Allocate memory for tokens
  char** tokens = calloc(count, sizeof(char*));
  if (tokens == NULL) return NULL;

  // Allocate memory for the string copy
  char* str_copy = strdup(str);
  if (str_copy == NULL) {
    free(tokens);
    *tokens_num = 0;
    return NULL;
  }

  // Tokenize the string
  char* token = _tok_next_token(str_copy, escapes, delims);

  for (size_t index = 0; token; index++) {
    tokens[index] = _strdup(token);
    token = _tok_next_token(NULL, escapes, delims);
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