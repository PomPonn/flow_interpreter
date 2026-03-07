#pragma once

#include "shared/common.h"

// fl_id_chain vector
#define FL_VECTOR_NAME fl_id_chain
#define FL_VECTOR_TYPE fl_char *
#define FL_VECTOR_CAP 4
#define FL_VECTOR_FREE_INNER FL_TRUE
#include "generics/vector.h"

fl_char *fl_id_chain_to_string(fl_id_chain *chain, fl_char *buffer, fl_size buffer_size);