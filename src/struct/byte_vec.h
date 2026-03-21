#pragma once

#include "shared/common.h"

#define _FL_BC_CAP_STEP 128

#define FL_VEC_NAME fl_byte_vec
#define FL_VEC_TYPE fl_byte
#define FL_VEC_CAP _FL_BC_CAP_STEP
#include "templates/vector.h"

FL_HEADER_FN fl_void fl_byte_vec_push_mem(fl_byte_vec *bc, const fl_unknown *mem, fl_size bytes_n)
{
    if (bc->size + bytes_n > bc->_capacity)
        fl_byte_vec_reserve(bc, bc->_capacity + bytes_n + _FL_BC_CAP_STEP);

    fl_mem_copy(mem, bc->data + bc->size, bytes_n);
    bc->size += bytes_n;
}