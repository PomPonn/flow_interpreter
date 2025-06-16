#pragma once

#include "common.h"

struct _tok_context {
  const char** escapes;
  const char** literal_wrappers;
  const char* delims;
  char* escapes_peek;

  size_t escapes_num;
  size_t literal_wrappers_num;
};
typedef struct _tok_context tok_context;

tok_context tok_make_context();

void tok_free_context(tok_context* context);

char** tok_tokenize(char* str, tok_context cnt, size_t* tokens_num);

void tok_free_tokens(char** tokens, size_t tokens_num);