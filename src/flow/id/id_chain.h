/**
 * ---------- id_chain.h ----------------
 *
 * Declares id chain logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ------------- MACROS -----------------

// Define fl_id_chain vector

#define FL_VEC_NAME fl_id_chain
#define FL_VEC_TYPE fl_char *
#define FL_VEC_CAP 3
#define FL_VEC_FREE_INNER FL_TRUE
#include "templates/vector.h"

// ---------- DECLARATIONS --------------

/**
 * @brief Parse string to id chain.
 * @param chain Empty id chain to put results in.
 * @param str String to parse.
 */
fl_void fl_id_chain_from_str(fl_id_chain *chain, const fl_char *str);

/**
 * @brief Convert id chain to string.
 * @param chain Id chain to convert.
 * @param buffer Char buffer to put id chain string in.
 * @param buffer_size Size of the buffer.
 * @return Pointer to the start of created string.
 */
fl_char *fl_id_chain_to_string(const fl_id_chain *chain, fl_char *buffer, fl_size buffer_size);