#include "compiler/byte_code.h"

#include "shared/memory.h"

#define _FL_SPARE_CAP 1024

fl_void fl_byte_code_init(fl_byte_code *bc, fl_size capacity)
{
    if (!capacity)
        capacity = _FL_SPARE_CAP;

    bc->data = (fl_byte *)fl_mem_alloc_arr(sizeof(fl_byte), capacity);
    bc->_capacity = capacity;
    bc->size = 0;
}

fl_void fl_byte_code_free(fl_byte_code *bc)
{
    fl_mem_free(bc->data);
    bc->data = FL_NULL;
    bc->_capacity = 0;
    bc->size = 0;
}

fl_void fl_byte_code_reserve(fl_byte_code *bc, fl_size new_cap)
{
    bc->_capacity = new_cap;
    fl_byte *recaped = (fl_byte *)fl_mem_alloc_arr(sizeof(fl_byte), bc->_capacity);
    fl_mem_copy(bc->data, recaped, bc->size);
}

fl_void fl_byte_code_push(fl_byte_code *bc, fl_byte byte)
{
    if (bc->size + sizeof(byte) > bc->_capacity)
        fl_byte_code_reserve(bc, bc->_capacity + 1 + _FL_SPARE_CAP);

    bc->data[bc->size] = byte;
    bc->size += sizeof(fl_byte);
}

fl_void fl_byte_code_push_mem(fl_byte_code *bc, const fl_unknown *mem, fl_size bytes_n)
{
    if (bc->size + bytes_n > bc->_capacity)
        fl_byte_code_reserve(bc, bc->_capacity + bytes_n + _FL_SPARE_CAP);

    fl_mem_copy(mem, bc->data + bc->size, bytes_n);
    bc->size += bytes_n;
}