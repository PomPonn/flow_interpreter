/**
 * ---------- vector.h --------------------
 *
 * Implements vector template logic.
 *
 * ----------------------------------------
 */

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "macros/basic.h"

// ------------- MACROS -----------------

#ifndef FL_VEC_NAME
#define FL_VEC_NAME fl_vector
#endif

#ifndef FL_VEC_TYPE
#define FL_VEC_TYPE fl_int
#endif

#ifndef FL_VEC_CAP
#define FL_VEC_CAP 16
#endif

#ifndef FL_VEC_CONST
#define FL_VEC_CONST const
#endif

#ifndef FL_VEC_COMPARE
#define FL_VEC_COMPARE(a, b) a == b
#endif

// Other properties set by a user:

// #define FL_VEC_DESTROY(elem)
// #define FL_VEC_EXCLUDE_FIND
// #define FL_VEC_NULL_VALUE

// ---------- TYPE DEFINITIONS ----------

struct _FL_MERGE2(_, FL_VEC_NAME)
{
    fl_size _capacity;
    fl_size size;
    FL_VEC_TYPE *data;
};
typedef struct _FL_MERGE2(_, FL_VEC_NAME) FL_VEC_NAME;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init_c)(FL_VEC_NAME *vec, fl_size capacity);
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init_s)(FL_VEC_NAME *vec, fl_size size);
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init)(FL_VEC_NAME *vec);
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init_empty)(FL_VEC_NAME *vec);
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init_with)(FL_VEC_NAME *vec, fl_size size, FL_VEC_TYPE *data);

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _free)(FL_VEC_NAME *vec);
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _free_shallow)(FL_VEC_NAME *vec);
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _reserve)(FL_VEC_NAME *vec, fl_size new_cap);
FL_HEADER_FN fl_bool _FL_MERGE2(FL_VEC_NAME, _is_empty)(const FL_VEC_NAME *vec);
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _put_back)(FL_VEC_NAME *vec, FL_VEC_TYPE value);
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _push)(FL_VEC_NAME *vec, FL_VEC_TYPE value);
FL_HEADER_FN FL_VEC_NAME _FL_MERGE2(FL_VEC_NAME, _copy)(FL_VEC_CONST FL_VEC_NAME *vec);
FL_HEADER_FN FL_VEC_TYPE _FL_MERGE2(FL_VEC_NAME, _pop)(FL_VEC_NAME *vec);
FL_HEADER_FN FL_VEC_TYPE _FL_MERGE2(FL_VEC_NAME, _peek)(FL_VEC_CONST FL_VEC_NAME *vec);

#ifndef FL_VEC_EXCLUDE_FIND

FL_HEADER_FN fl_int _FL_MERGE2(FL_VEC_NAME, _find)(const FL_VEC_NAME *vec, FL_VEC_CONST FL_VEC_TYPE elem);

#endif

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init_c)(FL_VEC_NAME *vec, fl_size capacity)
{
    vec->data = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), capacity);
    vec->_capacity = capacity;
    vec->size = 0;
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init_s)(FL_VEC_NAME *vec, fl_size size)
{
    vec->data = (FL_VEC_TYPE *)fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), size);
    vec->_capacity = size;
    vec->size = size;

    fl_mem_zero(vec->data, size * sizeof(FL_VEC_TYPE));
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init)(FL_VEC_NAME *vec)
{
    _FL_MERGE2(FL_VEC_NAME, _init_c)(vec, FL_VEC_CAP);
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init_empty)(FL_VEC_NAME *vec)
{
    vec->data = FL_NULL;
    vec->size = 0;
    vec->_capacity = 0;
}

// elem count of data must match size param
FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _init_with)(FL_VEC_NAME *vec, fl_size size, FL_VEC_TYPE *data)
{
    vec->data = fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), size);
    fl_mem_copy(data, vec->data, sizeof(FL_VEC_TYPE) * size);
    vec->_capacity = size;
    vec->size = size;
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
    vec->data = (FL_VEC_TYPE *)fl_mem_realloc_arr(vec->data, sizeof(FL_VEC_TYPE), new_cap);
    vec->_capacity = new_cap;
}

FL_HEADER_FN fl_bool _FL_MERGE2(FL_VEC_NAME, _is_empty)(const FL_VEC_NAME *vec)
{
    return vec->size == 0;
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _put_back)(FL_VEC_NAME *vec, FL_VEC_TYPE value)
{
    if (vec->size == vec->_capacity)
        _FL_MERGE2(FL_VEC_NAME, _reserve)(vec, vec->_capacity + FL_VEC_CAP);

    if (vec->size == 0)
    {
        vec->data[0] = value;
        vec->size++;
    }
    else
    {
        FL_VEC_TYPE *new_mem = fl_mem_alloc_arr(sizeof(FL_VEC_TYPE), vec->_capacity);
        new_mem[0] = value;
        fl_mem_copy(vec->data, &new_mem[1], sizeof(FL_VEC_TYPE) * vec->size++);
        fl_mem_free(vec->data);
        vec->data = new_mem;
    }
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _push)(FL_VEC_NAME *vec, FL_VEC_TYPE value)
{
    if (vec->size == vec->_capacity)
        _FL_MERGE2(FL_VEC_NAME, _reserve)(vec, vec->_capacity + FL_VEC_CAP);

    vec->data[vec->size] = value;
    vec->size++;
}

FL_HEADER_FN FL_VEC_NAME _FL_MERGE2(FL_VEC_NAME, _copy)(FL_VEC_CONST FL_VEC_NAME *vec)
{
    FL_VEC_NAME copied;
    _FL_MERGE2(FL_VEC_NAME, _init_c)(&copied, vec->size);
    copied.size = vec->size;
    fl_mem_copy(vec->data, copied.data, vec->size * sizeof(FL_VEC_TYPE));
    return copied;
}

FL_HEADER_FN FL_VEC_TYPE _FL_MERGE2(FL_VEC_NAME, _pop)(FL_VEC_NAME *vec)
{
#ifdef FL_VEC_NULL_VALUE
    if (vec->size == 0)
        return FL_VEC_NULL_VALUE;
#endif

    vec->size--;
    return vec->data[vec->size];
}

FL_HEADER_FN FL_VEC_TYPE _FL_MERGE2(FL_VEC_NAME, _peek)(FL_VEC_CONST FL_VEC_NAME *vec)
{
#ifdef FL_VEC_NULL_VALUE
    if (vec->size == 0)
        return FL_VEC_NULL_VALUE;
#endif

    return vec->data[vec->size - 1];
}

FL_HEADER_FN fl_void _FL_MERGE2(FL_VEC_NAME, _free_shallow)(FL_VEC_NAME *vec)
{
    fl_mem_free(vec->data);
    vec->data = FL_NULL;
    vec->_capacity = 0;
    vec->size = 0;
}

#ifndef FL_VEC_EXCLUDE_FIND

FL_HEADER_FN fl_int _FL_MERGE2(FL_VEC_NAME, _find)(const FL_VEC_NAME *vec, FL_VEC_CONST FL_VEC_TYPE elem)
{
    for (fl_size i = 0; i < vec->size; i++)
        if (FL_VEC_COMPARE(vec->data[i], elem))
            return (fl_int)i;

    return FL_FIND_FAIL;
}

#endif

#undef FL_VEC_NAME
#undef FL_VEC_TYPE
#undef FL_VEC_CAP
#undef FL_VEC_COMPARE
#undef FL_VEC_CONST

#ifdef FL_VEC_DESTROY
#undef FL_VEC_DESTROY
#endif
#ifdef FL_VEC_EXCLUDE_FIND
#undef FL_VEC_EXCLUDE_FIND
#endif
#ifdef FL_VEC_NULL_VALUE
#undef FL_VEC_NULL_VALUE
#endif