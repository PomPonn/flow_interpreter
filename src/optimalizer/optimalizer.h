#pragma once

#include "shared/common.h"
#include "shared/token.h"

// destroys old array
fl_token *fl_opt_optimalize(fl_token *tokens, fl_size tokens_num, fl_size *opted_num);