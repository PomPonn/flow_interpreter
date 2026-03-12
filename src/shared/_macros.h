#pragma once

#define _FL_MERGE_INNER(a, b) a##b
#define _FL_MERGE(a, b) _FL_MERGE_INNER(a, b)
#define _FL_MERGE3(a, b, c) _FL_MERGE(_FL_MERGE(a, b), c)
#define _FL_SSIZE(arr) (sizeof(arr) / sizeof(*arr))