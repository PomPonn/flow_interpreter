/**
 * ---------- tokenizer.h ----------------
 *
 * Declares tokenizer logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_token_arr);

// ---------- DECLARATIONS --------------

fl_bool fl_tok_tokenize(fl_char *source, fl_token_arr *token_arr, const fl_char *src_location);
