#pragma once

#include "shared/common.h"

struct _fl_byte_code
{
    fl_size _capacity;
    fl_size size;
    fl_byte *data;
};
typedef struct _fl_byte_code fl_byte_code;

fl_void fl_byte_code_init(fl_byte_code *bc, fl_size capacity);

fl_void fl_byte_code_free(fl_byte_code *bc);

fl_void fl_byte_code_reserve(fl_byte_code *bc, fl_size new_cap);

fl_void fl_byte_code_push(fl_byte_code *bc, fl_byte byte);

fl_void fl_byte_code_push_mem(fl_byte_code *bc, const fl_unknown *mem, fl_size bytes_n);