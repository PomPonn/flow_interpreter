#include "optimalizer.h"

#include "shared/token.h"
#include "shared/memory.h"

fl_void fl_opt_optimalize(fl_token_arr *arr)
{
    fl_token *opted;
    fl_size opted_size = arr->_count;
    fl_token_type last_type = FL_TOKEN_TYPE_NONE;

    fl_token_arr_reset_ptr(arr);
    while (arr->current)
    {
        if (arr->current->type == FL_TOKEN_TYPE_COMMENT || (arr->current->alias == FL_TOKEN_ALIAS_NL && (last_type == FL_TOKEN_TYPE_DELIMITER || last_type == FL_TOKEN_TYPE_NONE)))
        {
            fl_token_free(arr->current);
            opted_size--;
        }
        else if (arr->current->alias == FL_TOKEN_ALIAS_SPACE)
        {
            fl_token *peek = fl_token_arr_peek(arr);

            if (!peek || peek->alias != FL_TOKEN_ALIAS_DOT)
            {
                fl_token_free(arr->current);
                opted_size--;
            }
        }
        else
            last_type = arr->current->type;

        fl_token_arr_next(arr);
    }

    fl_size j = 0;
    last_type = FL_TOKEN_TYPE_NONE;
    opted = fl_mem_alloc_arr(opted_size, sizeof(fl_token));

    fl_token_arr_reset_ptr(arr);
    while (arr->current)
    {
        if (arr->current->type == FL_TOKEN_TYPE_COMMENT || (arr->current->alias == FL_TOKEN_ALIAS_NL && (last_type == FL_TOKEN_TYPE_DELIMITER || last_type == FL_TOKEN_TYPE_NONE)))
            continue;
        else if (arr->current->alias == FL_TOKEN_ALIAS_SPACE)
        {
            fl_token *peek = fl_token_arr_peek(arr);

            if (!peek || peek->alias != FL_TOKEN_ALIAS_DOT)
                continue;
        }

        last_type = arr->current->type;
        opted[j++] = *arr->current;

        fl_token_arr_next(arr);
    }

    fl_mem_free(arr->_data);
    fl_token_arr_init(arr, opted, opted_size);
}