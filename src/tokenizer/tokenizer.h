#pragma once

#include "shared/common.h"
#include "tokenizer/token_arr.h"
#include "vecs/cstr_vec.h"

fl_bool fl_tok_tokenize(
    fl_char *source,
    fl_token_arr *token_arr,
    const fl_cstr_vec *escapes,
    const fl_char *src_location);