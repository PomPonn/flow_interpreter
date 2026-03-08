#include "shared/common.h"
#include "shared/memory.h"

#ifndef FL_ARR_NAME
#define FL_ARR_NAME array
#endif
#ifndef FL_ARR_TYPE
#define FL_ARR_TYPE fl_int
#endif
#ifndef FL_ARR_PRIMITIVE
#define FL_ARR_PRIMITIVE FL_TRUE
#endif

#define _FL_MERGE_INNER(a, b) a##b
#define _FL_MERGE(a, b) _FL_MERGE_INNER(a, b)

struct _FL_MERGE(_, FL_ARR_NAME)
{
    fl_size size;
    FL_ARR_TYPE *data;
};
typedef struct _FL_MERGE(_, FL_ARR_NAME) FL_ARR_NAME;

inline fl_void _FL_MERGE(FL_ARR_NAME, _init)(FL_ARR_NAME *arr, fl_size size)
{
    arr->data = (FL_ARR_TYPE *)fl_mem_alloc_arr(sizeof(FL_ARR_TYPE), size);
    arr->size = size;
}

inline fl_void _FL_MERGE(FL_ARR_NAME, _free)(FL_ARR_NAME *arr)
{
#if FL_ARR_PRIMITIVE == 0
    for (fl_size i = 0; i < sizeof(FL_ARR_TYPE) * arr->size; i++)
        _FL_MERGE(FL_ARR_TYPE, _free)(&arr->data[i]);
#endif
    fl_mem_free(arr->data);
    arr->data = FL_NULL;
    arr->size = 0;
}

#undef FL_ARR_NAME
#undef FL_ARR_TYPE
#undef FL_ARR_PRIMITIVE