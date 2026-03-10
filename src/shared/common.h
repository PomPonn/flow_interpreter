#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdint.h>
#include <assert.h>
#include <limits.h>

#define FL_UINT_MAX UINT_MAX
#define FL_NULL (void *)0
#define FL_FAIL -1

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