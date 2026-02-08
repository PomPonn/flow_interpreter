#include "token_array.h"

#include "shared/memory.h"

fl_void fl_token_arr_init(fl_token_arr *arr, fl_token *data, fl_size token_count)
{
    arr->_count = token_count;
    arr->_data = data;
    arr->current = data;
    arr->_iterator = 0;
}

fl_void fl_token_arr_free(fl_token_arr *arr)
{
    if (!arr || !arr->_data)
        return;

    for (fl_size i = 0; i < arr->_count; i++)
        fl_token_free(arr->_data + i);

    fl_mem_free(arr->_data);
}

fl_token *fl_token_arr_next(fl_token_arr *arr)
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

fl_token *fl_token_arr_peek(fl_token_arr *arr)
{
    return arr->_iterator + 1 >= arr->_count ? FL_NULL : arr->current + 1;
}

fl_void fl_token_arr_reset_ptr(fl_token_arr *arr)
{
    arr->current = arr->_data;
    arr->_iterator = 0;
}