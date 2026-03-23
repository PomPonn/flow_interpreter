#include "io.h"

// ---- INCLUDES ----

#include "macros/modules.h"
#include <stdio.h>

// ---- DEFINES ----

#define FL_MODULE_NAME io

// ---- DECLARATIONS ----

FL_MODULE_CALLABLE(put);

// ---- DEFINITIONS ----

fl_void fl_std_io_register(fl_context *cnt, fl_id_chain *id_chain, fl_scope *scope)
{
    FL_REGISTER_START;

    FL_REGISTER_OBJ(scope, FL_MODULE_NAME);
    FL_REGISTER_CT_CALLABLE(
        put,
        FL_DATA_TYPE_NULL,
        FL_DATA_TYPE_DESC_DEFAULT(FL_DATA_TYPE_ARRAY));
}

FL_MODULE_CALLABLE(put)
{
    for (fl_size i = 0; i < args.size; i++)
    {
        // fl_call_param arg = args->data[i];
        // switch (arg.type)
        // {
        // case FL_DATA_TYPE_NULL:
        //     printf(_FL_NULL_STR_REPR);
        //     break;
        // case FL_DATA_TYPE_INT:
        //     printf("%lli", *(fl_long *)arg.data);
        //     break;
        // case FL_DATA_TYPE_BOOL:
        //     printf("%u", *(fl_uint *)arg.data);
        //     break;
        // case FL_DATA_TYPE_FLOAT:
        //     printf("%f", *(fl_double *)arg.data);
        //     break;
        // case FL_DATA_TYPE_ARRAY:
        //     // TODO
        //     printf("<ARRAY>");
        //     break;
        // case FL_DATA_TYPE_SCOPE:
        //     // TODO
        //     printf("<OBJECT>");
        //     break;
        // }
    }

    return FL_NULL;
}
