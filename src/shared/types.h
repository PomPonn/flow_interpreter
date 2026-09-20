/**
 * ---------- types.h --------------------
 *
 * Declares project types.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include <stdint.h>
#include <limits.h>

// ------------- MACROS -----------------

#define FL_SHORT_MAX INT16_MAX
#define FL_SHORT_MIN INT16_MIN

#define FL_FALSE 0
#define FL_TRUE 1

// ---------- TYPE DEFINITIONS ----------

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
typedef fl_byte fl_bool;

// language types

typedef fl_bool fl_flow_bool;
typedef fl_long fl_flow_int;
typedef fl_double fl_flow_float;