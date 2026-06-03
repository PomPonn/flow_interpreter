/**
 * ---------- value_vec.c --------------------
 *
 * Implements value vector logic.
 *
 * ------------------------------------------
 */

#include "value_vec.h"

// ---------- DEPENDENCIES --------------

#include "vecs/byte_vec.h"

// ---------- DEFINITIONS ---------------

fl_void fl_value_vec_to_bytes(const fl_value_vec *vec, fl_byte_vec *bytes)
{
    for (fl_size i = 0; i < vec->size; i++)
        fl_value_to_bytes(vec->data[i], bytes);
}
