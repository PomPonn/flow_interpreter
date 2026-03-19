#include "optimalizer.h"

fl_void fl_opt_remove_comments(fl_token_arr *arr)
{
    fl_size opted_size = arr->_count;

    for (fl_token_arr_reset_ptr(arr); arr->current; fl_token_arr_next(arr))
    {
        if (arr->current->type == FL_TOKEN_TYPE_COMMENT)
        {
            fl_token_free(arr->current);
            opted_size--;
        }
    }

    fl_size j = 0;
    fl_token_arr opted;
    fl_token_arr_init(&opted, opted_size);

    for (fl_token_arr_reset_ptr(arr); arr->current; fl_token_arr_next(arr))
    {
        if (arr->current->type == FL_TOKEN_TYPE_COMMENT)
            continue;

        opted._data[j++] = *arr->current;
    }

    fl_mem_free(arr->_data);
    *arr = opted;
}