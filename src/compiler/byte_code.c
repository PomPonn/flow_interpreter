#include "compiler/byte_code.h"

#include "shared/memory.h"

#define _FL_BYTE_CODE_CAP_STEP 1024

fl_void fl_byte_code_init(fl_byte_code *bc, fl_size capacity)
{
    if (!capacity)
        capacity = _FL_BYTE_CODE_CAP_STEP;

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

fl_void fl_byte_code_reserve(fl_byte_code *bc)
{
    bc->_capacity += _FL_BYTE_CODE_CAP_STEP;
    fl_byte *recaped = (fl_byte *)fl_mem_alloc_arr(sizeof(fl_byte), bc->_capacity);
    fl_mem_copy(bc->data, recaped, bc->size);
}

fl_void fl_byte_code_push_byte(fl_byte_code *bc, fl_byte byte)
{
    if (bc->size + sizeof(fl_byte) > bc->_capacity)
        fl_byte_code_reserve(bc);

    bc->data[bc->size] = byte;
    bc->size++;
}

fl_void fl_byte_code_push_int(fl_byte_code *bc, fl_uint value)
{
    if (bc->size + sizeof(fl_uint) > bc->_capacity)
        fl_byte_code_reserve(bc);

    *(fl_uint *)(bc->data + bc->size) = value;
    bc->size += sizeof(fl_uint);
}