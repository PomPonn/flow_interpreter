#include "shared/common.h"
#include "macros/basic.h"

#ifndef FL_VEC_NAME
#define FL_VEC_NAME fl_vector
#endif

#ifndef FL_VEC_TYPE
#define FL_VEC_TYPE fl_int
#endif

#ifndef FL_VEC_CAP
#define FL_VEC_CAP 16
#endif

#ifndef FL_VEC_COMPARE
#define FL_VEC_COMPARE(a, b) (a == b)
#endif

// #define FL_VEC_DESTROY(elem)
// #define FL_VEC_EXCLUDE_FIND

struct _FL_MERGE2(_, FL_VEC_NAME)
{
    fl_size _capacity;
    fl_size size;
    FL_VEC_TYPE *data;
};
typedef struct _FL_MERGE2(_, FL_VEC_NAME) FL_VEC_NAME;

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init)(FL_VEC_NAME *vec)
{
    vec->data = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), FL_VEC_CAP);
    vec->_capacity = FL_VEC_CAP;
    vec->size = 0;
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _initc)(FL_VEC_NAME *vec, fl_size capacity)
{
    if (capacity != 0)
        vec->data = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), capacity);

    vec->_capacity = capacity;
    vec->size = 0;
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _free)(FL_VEC_NAME *vec)
{

#ifdef FL_VEC_DESTROY
    for (fl_size i = 0; i < vec->size; i++)
        FL_VEC_DESTROY(vec->data[i]);
#endif

    fl_mem_free(vec->data);
    vec->data = FL_NULL;
    vec->_capacity = 0;
    vec->size = 0;
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _reserve)(FL_VEC_NAME *vec, fl_size new_cap)
{
    vec->_capacity = new_cap;
    FL_VEC_TYPE *recaped = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), vec->_capacity);
    fl_mem_copy(vec->data, recaped, sizeof(FL_VEC_TYPE) * vec->size);
    vec->data = recaped;
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _push)(FL_VEC_NAME *vec, FL_VEC_TYPE value)
{
    if (vec->size + 1 > vec->_capacity)
        _FL_MERGE2(FL_VEC_NAME, _reserve)(vec, vec->_capacity + 1 + FL_VEC_CAP);

    vec->data[vec->size] = value;
    vec->size++;
}

FL_HEADER_FN fl_bool _FL_MERGE2(FL_VEC_NAME, _is_empty)(const FL_VEC_NAME *vec)
{
    return vec->data == FL_NULL;
}

#ifndef FL_VEC_EXCLUDE_FIND

FL_HEADER_FN fl_int _FL_MERGE2(FL_VEC_NAME, _find)(const FL_VEC_NAME *vec, const FL_VEC_TYPE elem)
{
    for (fl_int i = 0; i < vec->size; i++)
        if (FL_VEC_COMPARE(vec->data[i], elem))
            return i;

    return -1;
}

#endif

#undef FL_VEC_NAME
#undef FL_VEC_TYPE
#undef FL_VEC_CAP
#undef FL_VEC_COMPARE

#ifdef FL_VEC_DESTROY
#undef FL_VEC_DESTROY
#endif
#ifdef FL_VEC_EXCLUDE_FIND
#undef FL_VEC_EXCLUDE_FIND
#endif
