#pragma once

#include "shared/common.h"

// fl_id_chain vector
#define FL_VEC_NAME fl_id_chain
#define FL_VEC_TYPE fl_char *
#define FL_VEC_CAP 3
#define FL_VEC_FREE_INNER FL_TRUE
#include "templates/vector.h"

fl_void fl_id_chain_init_parse(fl_id_chain *chain, const fl_char *str);

fl_char *fl_id_chain_to_string(fl_id_chain *chain, fl_char *buffer, fl_size buffer_size);