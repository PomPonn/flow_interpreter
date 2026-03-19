#include "shared/common.h"
#include "macros/basic.h"

#ifndef FL_ARR_NAME
#define FL_ARR_NAME fl_array
#endif
#ifndef FL_ARR_TYPE
#define FL_ARR_TYPE fl_int
#endif
// #define FL_ARR_DESTRUCTOR(elem)

struct _FL_MERGE2(_, FL_ARR_NAME)
{
    fl_size size;
    FL_ARR_TYPE *data;
};
typedef struct _FL_MERGE2(_, FL_ARR_NAME) FL_ARR_NAME;

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_ARR_NAME, _init)(FL_ARR_NAME *arr, fl_size size)
{
    arr->data = (FL_ARR_TYPE *)fl_mem_alloc_arr(sizeof(FL_ARR_TYPE), size);
    arr->size = size;
}

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_ARR_NAME, _free)(FL_ARR_NAME *arr)
{
#ifdef FL_ARR_DESTRUCTOR
    for (fl_size i = 0; i < arr->size; i++)
        FL_ARR_DESTRUCTOR(arr->data[i]);
#endif

    fl_mem_free(arr->data);
    arr->data = FL_NULL;
    arr->size = 0;
}

#undef FL_ARR_NAME
#undef FL_ARR_TYPE
#ifdef FL_ARR_DESTRUCTOR
#undef FL_ARR_DESTRUCTOR
#endif