#include "io.h"

#include "lang/context.h"

#include <stdio.h>

#define _FL_NULL_STR_REPR "<NULL>"

// DECLARATIONS

fl_unknown *_fl_std_io_put(fl_func_param_list *args);

// DEFINITIONS

fl_void fl_std_io_register(fl_context *cnt)
{
    // _fl_std_io_put
    fl_func_param_list put_params;
    fl_func_param_list_init(&put_params, 1);
    fl_func_param_init(&put_params.data[0], FL_DATA_TYPE_ARRAY);

    fl_func_entry put_func;
    fl_func_entry_init(
        &put_func,
        "put",
        _fl_std_io_put,
        FL_DATA_TYPE_NULL,
        put_params);

    fl_func_table_push(&cnt->func_table, put_func);
}

fl_unknown *_fl_std_io_put(fl_func_param_list *args)
{
    for (fl_size i = 0; i < args->size; i++)
    {
        fl_func_param arg = args->data[i];
        switch (arg.type)
        {
        case FL_DATA_TYPE_NULL:
            printf(_FL_NULL_STR_REPR);
            break;
        case FL_DATA_TYPE_INT:
            printf("%lli", *(fl_long *)arg.data);
            break;
        case FL_DATA_TYPE_BOOL:
            printf("%u", *(fl_uint *)arg.data);
            break;
        case FL_DATA_TYPE_FLOAT:
            printf("%f", *(fl_double *)arg.data);
            break;
        case FL_DATA_TYPE_ARRAY:
            // TODO
            printf("<ARRAY>");
            break;
        case FL_DATA_TYPE_OBJECT:
            // TODO
            printf("<OBJECT>");
            break;
        }
    }

    return FL_NULL;
}
