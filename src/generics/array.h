#include "shared/common.h"
#include "shared/memory.h"

#ifndef FL_ARRAY_NAME
#define FL_ARRAY_NAME array
#endif
#ifndef FL_ARRAY_TYPE
#define FL_ARRAY_TYPE fl_int
#endif
#ifndef FL_ARRAY_PRIMITIVE
#define FL_ARRAY_PRIMITIVE FL_TRUE
#endif

#define _FL_MERGE_INNER(a, b) a##b
#define _FL_MERGE(a, b) _FL_MERGE_INNER(a, b)

struct _FL_MERGE(_, FL_ARRAY_NAME)
{
    fl_size size;
    FL_ARRAY_TYPE *data;
};
typedef struct _FL_MERGE(_, FL_ARRAY_NAME) FL_ARRAY_NAME;

inline fl_void _FL_MERGE(FL_ARRAY_NAME, _init)(FL_ARRAY_NAME *arr, fl_size size)
{
    arr->data = (FL_ARRAY_TYPE *)fl_mem_alloc_arr(sizeof(FL_ARRAY_TYPE), size);
    arr->size = 0;
}

inline fl_void _FL_MERGE(FL_ARRAY_NAME, _free)(FL_ARRAY_NAME *arr)
{
#if FL_ARRAY_PRIMITIVE == 0
    for (fl_size i = 0; i < sizeof(FL_ARRAY_TYPE) * arr->size; i++)
        _FL_MERGE(FL_ARRAY_TYPE, _free)(&arr->data[i]);
#endif
    fl_mem_free(arr->data);
    arr->data = FL_NULL;
    arr->size = 0;
}

#undef FL_ARRAY_NAME
#undef FL_ARRAY_TYPE
#undef FL_ARRAY_PRIMITIVE