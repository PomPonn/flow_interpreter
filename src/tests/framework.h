#pragma once

#include "tools/file.h"
#include "tools/printer.h"
#include "optimalizer/optimalizer.h"
#include "tokenizer/tokenizer.h"
#include "shared/token_array.h"
#include "shared/error.h"

#include <stdio.h>

#define __prepare_tokens(token_arr_name)                                                \
    fl_token_arr token_arr_name;                                                        \
    char *file_content = fl_file_read(_TEST_INPUT_PATH);                                \
    if (!file_content)                                                                  \
    {                                                                                   \
        fprintf(stderr, "Failed to read file: %s\n", _TEST_INPUT_PATH);                 \
        return 1;                                                                       \
    }                                                                                   \
    {                                                                                   \
        fl_bool res = fl_tok_tokenize(file_content, &token_arr_name, _TEST_INPUT_PATH); \
        if (!res)                                                                       \
        {                                                                               \
            fl_file_free(file_content);                                                 \
            fprintf(stderr, "Failed to tokenize content.\n");                           \
            return 1;                                                                   \
        }                                                                               \
    }                                                                                   \
    fl_opt_remove_comments(&token_array);

#define __free_prepared(token_arr_name) \
    fl_file_free(file_content);         \
    fl_token_arr_free(&token_arr_name);