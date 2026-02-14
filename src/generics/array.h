#include "shared/common.h"
#include "shared/memory.h"

#ifndef FL_ARRAY_NAME

#define FL_ARRAY_NAME array
#define FL_ARRAY_TYPE int
#define FL_ARRAY_CAP_STEP 16

#endif

#define _FL_MERGE_INNER(a, b) a##b
#define _FL_MERGE(a, b) _FL_MERGE_INNER(a, b)
#define _FL_NAME_PREFIXED _FL_MERGE(fl_, FL_ARRAY_NAME)

struct _FL_MERGE(_, _FL_NAME_PREFIXED)
{
    fl_size capacity;
    fl_size size;
    FL_ARRAY_TYPE *data;
};
typedef struct _FL_MERGE(_, _FL_NAME_PREFIXED) _FL_NAME_PREFIXED;

inline fl_void _FL_MERGE(_FL_NAME_PREFIXED, _init)(_FL_NAME_PREFIXED *arr, fl_size capacity)
{
    arr->data = (FL_ARRAY_TYPE *)fl_mem_alloc_arr(sizeof(FL_ARRAY_TYPE), capacity);
    arr->capacity = capacity;
    arr->size = 0;
}

inline fl_void _FL_MERGE(_FL_NAME_PREFIXED, _reserve)(_FL_NAME_PREFIXED *arr)
{
    arr->capacity += FL_ARRAY_CAP_STEP;
    FL_ARRAY_TYPE *recaped = (FL_ARRAY_TYPE *)fl_mem_alloc_arr(sizeof(FL_ARRAY_TYPE), arr->capacity);
    fl_mem_copy(arr->data, recaped, arr->size);
}

inline fl_void _FL_MERGE(_FL_NAME_PREFIXED, _push)(_FL_NAME_PREFIXED *arr, FL_ARRAY_TYPE value)
{
    if (arr->size + 1 > arr->capacity)
        _FL_MERGE(_FL_NAME_PREFIXED, _reserve)(arr);

    arr->data[arr->size] = value;
    arr->size++;
}

#undef FL_ARRAY_NAME
#undef FL_ARRAY_TYPE
#undef FL_ARRAY_CAP_STEP