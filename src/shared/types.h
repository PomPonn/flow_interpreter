#pragma once

#include <stdint.h>
#include <limits.h>

#define FL_INT_MAX INT_MAX

typedef void fl_unknown;
typedef void fl_void;
typedef char fl_char;
typedef double fl_double;
typedef uint8_t fl_byte;
typedef uint16_t fl_ushort;
typedef int16_t fl_short;
typedef uint32_t fl_uint;
typedef int32_t fl_int;
typedef uint64_t fl_ulong;
typedef int64_t fl_long;
typedef fl_uint fl_size;

enum _fl_bool
{
    FL_FALSE,
    FL_TRUE,
};
typedef fl_byte fl_bool;

typedef fl_long fl_flow_type_int;
typedef fl_double fl_flow_type_float;