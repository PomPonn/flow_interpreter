/**
 * ---------- token_arr.h -------------
 *
 * Declares token array logic.
 *
 * ------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES -------------

#include "shared/common.h"
#include "tokenizer/token.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_token_arr
{
    fl_token *current;
    fl_token *_data;
    fl_size _iterator;
    fl_size _count;
};
typedef struct _fl_token_arr fl_token_arr;

// ---------- DECLARATIONS --------------

FL_INLINE_FN fl_token_arr fl_token_arr_create_empty();
FL_INLINE_FN fl_void fl_token_arr_init(fl_token_arr *arr, fl_size count);
FL_INLINE_FN fl_void fl_token_arr_free(fl_token_arr *arr);
FL_INLINE_FN fl_token *fl_token_arr_next(fl_token_arr *arr);
FL_INLINE_FN fl_void fl_token_arr_reset_ptr(fl_token_arr *arr);
FL_INLINE_FN fl_token *fl_token_arr_peek(const fl_token_arr *arr);
FL_INLINE_FN fl_bool fl_token_arr_is_empty(const fl_token_arr *arr);

// ---------- DEFINITIONS -----------

FL_INLINE_FN fl_token_arr fl_token_arr_create_empty()
{
    return (fl_token_arr){
        .current = FL_NULL,
        ._data = FL_NULL,
        ._iterator = 0,
        ._count = 0,
    };
}

FL_INLINE_FN fl_void fl_token_arr_init(fl_token_arr *arr, fl_size count)
{
    arr->_data = fl_mem_alloc_arr(sizeof(fl_token), count);
    arr->current = &arr->_data[0];
    arr->_count = count;
    arr->_iterator = 0;
}

FL_INLINE_FN fl_void fl_token_arr_free(fl_token_arr *arr)
{
    if (!arr->_data)
        return;

    for (fl_size i = 0; i < arr->_count; i++)
        fl_token_free(arr->_data + i);

    fl_mem_free(arr->_data);
}

FL_INLINE_FN fl_token *fl_token_arr_next(fl_token_arr *arr)
{
    if (arr->_iterator + 1 >= arr->_count)
    {
        arr->current = FL_NULL;
        return FL_NULL;
    }
    else
    {
        arr->_iterator++;
        return ++arr->current;
    }
}

FL_INLINE_FN fl_token *fl_token_arr_peek(const fl_token_arr *arr)
{
    return arr->_iterator + 1 >= arr->_count ? FL_NULL : arr->current + 1;
}

FL_INLINE_FN fl_void fl_token_arr_reset_ptr(fl_token_arr *arr)
{
    arr->current = arr->_data;
    arr->_iterator = 0;
}

FL_INLINE_FN fl_bool fl_token_arr_is_empty(const fl_token_arr *arr)
{
    return arr->_count == 0;
}