#pragma once

#include "shared/token.h"

struct _fl_token_arr
{
    fl_token *current;
    fl_token *_data;
    fl_size _iterator;
    fl_size _count;
};
typedef struct _fl_token_arr fl_token_arr;

fl_void fl_token_arr_init(fl_token_arr *arr, fl_token *data, fl_size token_count);

fl_void fl_token_arr_free(fl_token_arr *arr);

fl_token *fl_token_arr_next(fl_token_arr *arr);

fl_token *fl_token_arr_peek(fl_token_arr *arr);

fl_void *fl_token_arr_reset_ptr(fl_token_arr *arr);