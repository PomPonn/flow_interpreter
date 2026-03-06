#pragma once

#include "shared/common.h"

#define fl_byte_code_push(...) fl_byte_code_push_byte(__VA_ARGS__)

struct _fl_byte_code
{
    fl_size _capacity;
    fl_size size;
    fl_byte *data;
};
typedef struct _fl_byte_code fl_byte_code;

fl_void fl_byte_code_init(fl_byte_code *bc, fl_size capacity);

fl_void fl_byte_code_free(fl_byte_code *bc);

fl_void fl_byte_code_reserve(fl_byte_code *bc);

fl_void fl_byte_code_push_byte(fl_byte_code *bc, fl_byte value);

fl_void fl_byte_code_push_int(fl_byte_code *bc, fl_uint value);