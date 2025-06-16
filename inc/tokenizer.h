#pragma once

#include "common.h"

struct _tok_context {
  const char** escapes;
  char* escapes_peek;
  const char* delims;

  size_t escapes_num;
};
typedef struct _tok_context tok_context;

tok_context tok_make_context();

void tok_free_context(tok_context* context);

char** tok_tokenize(char* str, tok_context cnt, size_t* tokens_num);

void tok_free_tokens(char** tokens, size_t tokens_num);