#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdint.h>
#include <assert.h>

#define FL_NULL (void *)0

#define FL_ORDERED_DELIMS ",.:;(){}[] \n"
#define FL_WHITESPACE " \n\t"
#define FL_ESCAPE_CHAR '\\'
#define FL_STRING_WRAPPER '"'
#define FL_COMMENT_WRAPPER '#'
#define FL_WRAPPERS "\"#"

typedef void fl_unknown;
typedef char fl_char;
typedef uint8_t fl_ubyte;
typedef int8_t fl_byte;
typedef uint16_t fl_ushort;
typedef int16_t fl_short;
typedef uint32_t fl_uint;
typedef int32_t fl_int;
typedef uint64_t fl_ulong;
typedef int64_t fl_long;
typedef fl_ulong fl_size;

enum _fl_bool
{
    FL_FALSE,
    FL_TRUE,
};
typedef fl_ubyte fl_bool;