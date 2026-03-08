#include "io.h"

#include "shared/error.h"

#include <stdio.h>

// ---- DEFINES ----

#define _FL_NULL_STR_REPR "<NULL>"

// ---- DECLARATIONS ----

fl_unknown *_fl_std_io_put(fl_fn_param_list *args);

// ---- DEFINITIONS ----

fl_bool fl_std_io_register(fl_context *cnt, fl_id_tree_node *parent)
{
    // register 'io' module

    fl_obj_entry *io_object;
    fl_obj_entry_init(&io_object, "io");
    parent = fl_id_tree_push_child(parent, (fl_id_entry *)io_object);

    if (!parent)
        // TODO: report an error here and in other places
        // object with this name already exists
        return FL_FALSE;

    // register 'put' function (_fl_std_io_put)

    fl_fn_param_list put_params;
    fl_fn_param_list_init(&put_params, 1);
    fl_fn_param_init(&put_params.data[0], FL_DATA_TYPE_ARRAY);

    fl_fn_entry *put_func;
    fl_fn_entry_init(
        &put_func,
        "put",
        _fl_std_io_put,
        FL_DATA_TYPE_NULL,
        put_params);

    fl_context_register_func(cnt, parent, put_func);

    return FL_TRUE;
}

fl_unknown *_fl_std_io_put(fl_fn_param_list *args)
{
    for (fl_size i = 0; i < args->size; i++)
    {
        fl_fn_param arg = args->data[i];
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
