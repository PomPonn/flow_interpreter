#pragma once

#include "shared/common.h"
#include "vecs/byte_vec.h"
#include "shared/str.h"

// fl_id_chain vector
#define FL_VEC_NAME fl_id_chain
#define FL_VEC_TYPE fl_char *
#define FL_VEC_CAP 3
#define FL_VEC_FREE_INNER FL_TRUE
#include "templates/vector.h"

fl_void fl_id_chain_init_parse(fl_id_chain *chain, const fl_char *str);

fl_char *fl_id_chain_to_string(fl_id_chain *chain, fl_char *buffer, fl_size buffer_size);

// FL_HEADER_FN fl_size fl_id_chain_str_size(fl_id_chain *chain)
// {
//     fl_size sum = 0;
//     for (fl_size i = 0; i < chain->size; i++)
//         sum += fl_str_len(chain->data[i]);

//     // add dots count to string size sum
//     return sum + chain->size - 1;
// }

// FL_HEADER_FN fl_char *fl_id_chain_to_bytes(fl_id_chain *id_chain, fl_byte_vec *bytes)
// {
// }