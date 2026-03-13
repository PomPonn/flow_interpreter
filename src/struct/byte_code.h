#pragma once

#include "shared/common.h"

#define _FL_BC_CAP_STEP 1024

// fl_id_chain vector
#define FL_VEC_NAME fl_byte_code
#define FL_VEC_TYPE fl_byte
#define FL_VEC_CAP _FL_BC_CAP_STEP
#include "templates/vector.h"

fl_void fl_byte_code_push_mem(fl_byte_code *bc, const fl_unknown *mem, fl_size bytes_n);