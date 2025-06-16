#pragma once

#include "common.h"

uint8_t tok_is_escape_next(const char* str, const char** escapes,
                           size_t escapes_num, size_t* escape_len);

char** tok_tokenize(char* str, const char** escapes, size_t escapes_num,
                    const char* delims, size_t* tokens_num);

void tok_free_tokens(char** tokens, size_t tokens_num);