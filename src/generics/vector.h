#include "shared/common.h"
#include "shared/memory.h"

#ifndef FL_VECTOR_NAME
#define FL_VECTOR_NAME vector
#endif
#ifndef FL_VECTOR_TYPE
#define FL_VECTOR_TYPE fl_int
#endif
#ifndef FL_VECTOR_CAP
#define FL_VECTOR_CAP 16
#endif
#ifndef FL_VECTOR_STRUCTS
#define FL_VECTOR_STRUCTS 0
#endif
#ifndef FL_VECTOR_FREE_INNER
#define FL_VECTOR_FREE_INNER 0
#endif

#define _FL_MERGE_INNER(a, b) a##b
#define _FL_MERGE(a, b) _FL_MERGE_INNER(a, b)

struct _FL_MERGE(_, FL_VECTOR_NAME)
{
    fl_size _capacity;
    fl_size size;
    FL_VECTOR_TYPE *data;
};
typedef struct _FL_MERGE(_, FL_VECTOR_NAME) FL_VECTOR_NAME;

inline fl_void _FL_MERGE(FL_VECTOR_NAME, _init)(FL_VECTOR_NAME *arr, fl_size capacity)
{
    if (!capacity)
        capacity = FL_VECTOR_CAP;

    arr->data = (FL_VECTOR_TYPE *)fl_mem_alloc_arr(sizeof(FL_VECTOR_TYPE), capacity);
    arr->_capacity = FL_VECTOR_CAP;
    arr->size = 0;
}

inline fl_void _FL_MERGE(FL_VECTOR_NAME, _free)(FL_VECTOR_NAME *arr)
{
#if FL_VECTOR_STRUCTS == 1
    for (fl_size i = 0; i < sizeof(FL_VECTOR_TYPE) * arr->size; i++)
        _FL_MERGE(FL_VECTOR_TYPE, _free)(&arr->data[i]);
#elif FL_VECTOR_FREE_INNER == 1
    for (fl_size i = 0; i < sizeof(FL_VECTOR_TYPE) * arr->size; i++)
        fl_mem_free(arr->data[i]);
#endif
    fl_mem_free(arr->data);
    arr->data = FL_NULL;
    arr->_capacity = 0;
    arr->size = 0;
}

inline fl_void _FL_MERGE(FL_VECTOR_NAME, _reserve)(FL_VECTOR_NAME *arr, fl_size new_cap)
{
    arr->_capacity = new_cap;
    FL_VECTOR_TYPE *recaped = (FL_VECTOR_TYPE *)fl_mem_alloc_arr(sizeof(FL_VECTOR_TYPE), arr->_capacity);
    fl_mem_copy(arr->data, recaped, arr->size);
    arr->data = recaped;
}

inline fl_void _FL_MERGE(FL_VECTOR_NAME, _push)(FL_VECTOR_NAME *arr, FL_VECTOR_TYPE value)
{
    if (arr->size + 1 > arr->_capacity)
        _FL_MERGE(FL_VECTOR_NAME, _reserve)(arr, arr->_capacity + 1 + FL_VECTOR_CAP);

    arr->data[arr->size] = value;
    arr->size++;
}

#undef FL_VECTOR_NAME
#undef FL_VECTOR_TYPE
#undef FL_VECTOR_CAP
#undef FL_VECTOR_STRUCTS
#undef FL_VECTOR_FREE_INNER