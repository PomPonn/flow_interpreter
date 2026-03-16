#include "shared/common.h"
#include "macros/basic.h"

#ifndef FL_VEC_NAME
#define FL_VEC_NAME vector
#endif
#ifndef FL_VEC_TYPE
#define FL_VEC_TYPE fl_int
#endif
#ifndef FL_VEC_CAP
#define FL_VEC_CAP 16
#endif
// #define FL_VEC_DESTRUCTOR(elem)

struct _FL_MERGE2(_, FL_VEC_NAME)
{
    fl_size _capacity;
    fl_size size;
    FL_VEC_TYPE *data;
};
typedef struct _FL_MERGE2(_, FL_VEC_NAME) FL_VEC_NAME;

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init)(FL_VEC_NAME *arr, fl_size capacity)
{
    if (!capacity)
        capacity = FL_VEC_CAP;

    arr->data = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), capacity);
    arr->_capacity = capacity;
    arr->size = 0;
}

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_VEC_NAME, _free)(FL_VEC_NAME *arr)
{
#ifdef FL_VEC_DESTRUCTOR
    for (fl_size i = 0; i < sizeof(FL_VEC_TYPE) * arr->size; i++)
        FL_VEC_DESTRUCTOR(arr->data[i]);
#endif

    fl_mem_free(arr->data);
    arr->data = FL_NULL;
    arr->_capacity = 0;
    arr->size = 0;
}

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_VEC_NAME, _reserve)(FL_VEC_NAME *arr, fl_size new_cap)
{
    arr->_capacity = new_cap;
    FL_VEC_TYPE *recaped = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), arr->_capacity);
    fl_mem_copy(arr->data, recaped, arr->size);
    arr->data = recaped;
}

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_VEC_NAME, _push)(FL_VEC_NAME *arr, FL_VEC_TYPE value)
{
    if (arr->size + 1 > arr->_capacity)
        _FL_MERGE2(FL_VEC_NAME, _reserve)(arr, arr->_capacity + 1 + FL_VEC_CAP);

    arr->data[arr->size] = value;
    arr->size++;
}

FL_TEMPLATE_FN fl_int _FL_MERGE2(FL_VEC_NAME, _find)(FL_VEC_NAME *arr, const FL_VEC_TYPE value)
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
#ifdef FL_VEC_DESTRUCTOR
#undef FL_VEC_DESTRUCTOR
#endif