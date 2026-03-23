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
// #define FL_VEC_DESTRUCTOR(elem)

struct _FL_MERGE2(_, FL_VEC_NAME)
{
    fl_size _capacity;
    fl_size size;
    FL_VEC_TYPE *data;
};
typedef struct _FL_MERGE2(_, FL_VEC_NAME) FL_VEC_NAME;

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init)(FL_VEC_NAME *vec, fl_size capacity)
{
    if (!capacity)
        capacity = FL_VEC_CAP;

    vec->data = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), capacity);
    vec->_capacity = capacity;
    vec->size = 0;
}

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_VEC_NAME, _free)(FL_VEC_NAME *vec)
{
#ifdef FL_VEC_DESTRUCTOR
    for (fl_size i = 0; i < vec->size; i++)
        FL_VEC_DESTRUCTOR(vec->data[i]);
#endif

    fl_mem_free(vec->data);
    vec->data = FL_NULL;
    vec->_capacity = 0;
    vec->size = 0;
}

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_VEC_NAME, _reserve)(FL_VEC_NAME *vec, fl_size new_cap)
{
    vec->_capacity = new_cap;
    FL_VEC_TYPE *recaped = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), vec->_capacity);
    fl_mem_copy(vec->data, recaped, sizeof(FL_VEC_TYPE) * vec->size);
    vec->data = recaped;
}

FL_TEMPLATE_FN fl_void _FL_MERGE2(FL_VEC_NAME, _push)(FL_VEC_NAME *vec, FL_VEC_TYPE value)
{
    if (vec->size + 1 > vec->_capacity)
        _FL_MERGE2(FL_VEC_NAME, _reserve)(vec, vec->_capacity + 1 + FL_VEC_CAP);

    vec->data[vec->size] = value;
    vec->size++;
}

FL_TEMPLATE_FN fl_bool _FL_MERGE2(FL_VEC_NAME, _is_empty)(FL_VEC_NAME *vec)
{
    return vec->data == FL_NULL;
}

// FL_TEMPLATE_FN fl_int _FL_MERGE2(FL_VEC_NAME, _find)(FL_VEC_NAME *vec, const FL_VEC_TYPE value)
// {
//     for (fl_size i = 0; i < vec->size; i++)
//     {
//         if (vec->data[i] == value)
//             return (fl_int)i;
//     }

//     return -1;
// }

#undef FL_VEC_NAME
#undef FL_VEC_TYPE
#undef FL_VEC_CAP
#ifdef FL_VEC_DESTRUCTOR
#undef FL_VEC_DESTRUCTOR
#endif