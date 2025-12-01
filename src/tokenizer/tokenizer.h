#pragma once

#include "shared/common.h"
#include "shared/token.h"

fl_token *fl_tok_tokenize(fl_char *str, fl_size *tokens_num);

void fl_tok_free_tokens(fl_token *tokens, fl_size tokens_num);