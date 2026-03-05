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
#ifndef FL_VECTOR_PRIMITIVE
#define FL_VECTOR_PRIMITIVE 1
#endif

#define _FL_MERGE_INNER(a, b) a##b
#define _FL_MERGE(a, b) _FL_MERGE_INNER(a, b)
#define _FL_NAME_PREFIXED _FL_MERGE(fl_, FL_VECTOR_NAME)

struct _FL_MERGE(_, _FL_NAME_PREFIXED)
{
    fl_size _capacity;
    fl_size size;
    FL_VECTOR_TYPE *data;
};
typedef struct _FL_MERGE(_, _FL_NAME_PREFIXED) _FL_NAME_PREFIXED;

inline fl_void _FL_MERGE(_FL_NAME_PREFIXED, _init)(_FL_NAME_PREFIXED *arr)
{
    arr->data = (FL_VECTOR_TYPE *)fl_mem_alloc_arr(sizeof(FL_VECTOR_TYPE), FL_VECTOR_CAP);
    arr->_capacity = FL_VECTOR_CAP;
    arr->size = 0;
}

inline fl_void _FL_MERGE(_FL_NAME_PREFIXED, _free)(_FL_NAME_PREFIXED *arr)
{
#if FL_VECTOR_PRIMITIVE == 0
    for (fl_size i = 0; i < sizeof(FL_VECTOR_TYPE) * arr->size; i++)
        _FL_MERGE(FL_VECTOR_TYPE, _free)(&arr->data[i]);
#endif
    fl_mem_free(arr->data);
    arr->data = FL_NULL;
    arr->_capacity = 0;
    arr->size = 0;
}

inline fl_void _FL_MERGE(_FL_NAME_PREFIXED, _reserve)(_FL_NAME_PREFIXED *arr)
{
    arr->_capacity += FL_VECTOR_CAP;
    FL_VECTOR_TYPE *recaped = (FL_VECTOR_TYPE *)fl_mem_alloc_arr(sizeof(FL_VECTOR_TYPE), arr->_capacity);
    fl_mem_copy(arr->data, recaped, arr->size);
}

inline fl_void _FL_MERGE(_FL_NAME_PREFIXED, _push)(_FL_NAME_PREFIXED *arr, FL_VECTOR_TYPE value)
{
    if (arr->size + 1 > arr->_capacity)
        _FL_MERGE(_FL_NAME_PREFIXED, _reserve)(arr);

    arr->data[arr->size] = value;
    arr->size++;
}

#undef FL_VECTOR_NAME
#undef FL_VECTOR_TYPE
#undef FL_VECTOR_CAP
#undef FL_VECTOR_PRIMITIVE