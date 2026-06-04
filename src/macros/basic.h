/**
 * ---------- macros/basic.h --------------
 *
 * Declares basic macros.
 *
 * ----------------------------------------
 */

#pragma once

// ------------- MACROS -----------------

#define _FL_MERGE_HELPER(a, b) a##b
#define _FL_MERGE2(a, b) _FL_MERGE_HELPER(a, b)
#define _FL_MERGE3(a, b, c) _FL_MERGE2(_FL_MERGE2(a, b), c)
#define _FL_MERGE4(a, b, c, d) _FL_MERGE3(a, b, _FL_MERGE2(c, d))
#define _FL_SSIZE(arr) (sizeof(arr) / sizeof(*arr))
#define _FL_STRINGIFY_HELPER(x) #x
#define _FL_STRINGIFY(x) _FL_STRINGIFY_HELPER(x)