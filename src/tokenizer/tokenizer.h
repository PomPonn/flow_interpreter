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

fl_token_arr fl_tok_tokenize(fl_char *source, const fl_char *src_location);
