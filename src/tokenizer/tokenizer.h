#pragma once

#include "shared/common.h"
#include "shared/token.h"
#include "shared/token_array.h"

fl_bool fl_tok_tokenize(fl_char *str, fl_token_arr *token_arr, const fl_char *source_desc);