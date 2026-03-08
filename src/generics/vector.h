#include "shared/common.h"
#include "shared/memory.h"

#ifndef FL_VEC_NAME
#define FL_VEC_NAME vector
#endif
#ifndef FL_VEC_TYPE
#define FL_VEC_TYPE fl_int
#endif
#ifndef FL_VEC_CAP
#define FL_VEC_CAP 16
#endif
#ifndef FL_VEC_STRUCTS
#define FL_VEC_STRUCTS 0
#endif
#ifndef FL_VEC_FREE_INNER
#define FL_VEC_FREE_INNER 0
#endif

#define _FL_MERGE_INNER(a, b) a##b
#define _FL_MERGE(a, b) _FL_MERGE_INNER(a, b)

struct _FL_MERGE(_, FL_VEC_NAME)
{
    fl_size _capacity;
    fl_size size;
    FL_VEC_TYPE *data;
};
typedef struct _FL_MERGE(_, FL_VEC_NAME) FL_VEC_NAME;

inline fl_void _FL_MERGE(FL_VEC_NAME, _init)(FL_VEC_NAME *arr, fl_size capacity)
{
    if (!capacity)
        capacity = FL_VEC_CAP;

    arr->data = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), capacity);
    arr->_capacity = capacity;
    arr->size = 0;
}

inline fl_void _FL_MERGE(FL_VEC_NAME, _free)(FL_VEC_NAME *arr)
{
#if FL_VEC_STRUCTS == 1
    for (fl_size i = 0; i < sizeof(FL_VEC_TYPE) * arr->size; i++)
        _FL_MERGE(FL_VEC_TYPE, _free)(&arr->data[i]);
#elif FL_VEC_FREE_INNER == 1
    for (fl_size i = 0; i < sizeof(FL_VEC_TYPE) * arr->size; i++)
        fl_mem_free(arr->data[i]);
#endif
    fl_mem_free(arr->data);
    arr->data = FL_NULL;
    arr->_capacity = 0;
    arr->size = 0;
}

inline fl_void _FL_MERGE(FL_VEC_NAME, _reserve)(FL_VEC_NAME *arr, fl_size new_cap)
{
    arr->_capacity = new_cap;
    FL_VEC_TYPE *recaped = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), arr->_capacity);
    fl_mem_copy(arr->data, recaped, arr->size);
    arr->data = recaped;
}

inline fl_void _FL_MERGE(FL_VEC_NAME, _push)(FL_VEC_NAME *arr, FL_VEC_TYPE value)
{
    if (arr->size + 1 > arr->_capacity)
        _FL_MERGE(FL_VEC_NAME, _reserve)(arr, arr->_capacity + 1 + FL_VEC_CAP);

    arr->data[arr->size] = value;
    arr->size++;
}

inline fl_int _FL_MERGE(FL_VEC_NAME, _find)(FL_VEC_NAME *arr, FL_VEC_TYPE value)
{
    for (fl_size i = 0; i < arr->size; i++)
    {
        if (arr->data[i] == value)
            return (fl_int)i;
    }

    return -1;
}

#undef FL_VEC_NAME
#undef FL_VEC_TYPE
#undef FL_VEC_CAP
#undef FL_VEC_STRUCTS
#undef FL_VEC_FREE_INNER