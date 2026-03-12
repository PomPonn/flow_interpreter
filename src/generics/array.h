#include "shared/common.h"
#include "_macros.h"

#ifndef FL_ARR_NAME
#define FL_ARR_NAME array
#endif
#ifndef FL_ARR_TYPE
#define FL_ARR_TYPE fl_int
#endif
// #define FL_ARR_DESTROYER(elem)

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
#ifdef FL_ARR_DESTROYER
    for (fl_size i = 0; i < sizeof(FL_ARR_TYPE) * arr->size; i++)
        FL_ARR_DESTROYER(arr->data[i]);
#endif

    fl_mem_free(arr->data);
    arr->data = FL_NULL;
    arr->size = 0;
}

#undef FL_ARR_NAME
#undef FL_ARR_TYPE
#ifdef FL_ARR_DESTROYER
#undef FL_ARR_DESTROYER
#endif