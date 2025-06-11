#pragma once

#include "common.h"

char** tok_tokenize(const char* str, const char* escapes, const char* delims,
                    size_t* tokens_num);

void tok_free_tokens(char** tokens, size_t tokens_num);