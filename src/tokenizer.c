#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

static size_t _tok_count_tokens(const char* str, const char* delims) {
  if (str == NULL || delims == NULL) return 0;

  size_t count = 0;
  char* ptr = strpbrk(str, delims);

  while (ptr != NULL) {
    count++;
    while (strchr(delims, *ptr) != NULL && *ptr != '\0') ptr++;

    ptr = strpbrk(ptr + 1, delims);
  }

  return count + 1;
}

char* _tok_next_token(char* str, const char* escapes, const char* delims) {
  if (str == NULL || delims == NULL) return NULL;

  const char* start = str;

  const char* ptr = strpbrk(str, escapes);
}

char** tok_tokenize(const char* str, const char* delims, size_t* tokens_num) {
  // validate arguments
  if (str == NULL || delims == NULL || tokens_num == NULL) return NULL;

  // count tokens
  size_t count = _tok_count_tokens(str, delims);

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
  char* token = strtok(str_copy, delims);

  for (size_t index = 0; token != NULL;) {
    tokens[index++] = _strdup(token);
    token = strtok(NULL, delims);
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