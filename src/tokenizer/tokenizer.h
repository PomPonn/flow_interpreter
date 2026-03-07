#pragma once

#include "tokenizer/token_array.h"
#include "shared/common.h"

fl_bool fl_tok_tokenize(fl_char *str, fl_token_arr *token_arr, const fl_char *source_desc);