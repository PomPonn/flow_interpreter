#include "byte_code.h"

fl_void fl_byte_code_push_mem(fl_byte_code *bc, const fl_unknown *mem, fl_size bytes_n)
{
    if (bc->size + bytes_n > bc->_capacity)
        fl_byte_code_reserve(bc, bc->_capacity + bytes_n + _FL_BC_CAP_STEP);

    fl_mem_copy(mem, bc->data + bc->size, bytes_n);
    bc->size += bytes_n;
}