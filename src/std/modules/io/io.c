#include "std/modules/io/io.h"

#include "std/lib.h"

#include <stdio.h>

#define _FL_NULL_STR_REPR "<NULL>"

fl_void *_fl_std_io_put(fl_lib_params *args)
{
    for (fl_size i = 0; i < args->size; i++)
    {
        fl_lib_param arg = args->data[i];
        switch (arg.type)
        {
        case FL_LIB_TYPE_NULL:
            printf(_FL_NULL_STR_REPR);
            break;
        case FL_LIB_TYPE_INT:
            printf("%lli", *(fl_long *)arg.data);
            break;
        case FL_LIB_TYPE_BOOL:
            printf("%u", *(fl_uint *)arg.data);
            break;
        case FL_LIB_TYPE_FLOAT:
            printf("%f", *(fl_double *)arg.data);
            break;
        case FL_LIB_TYPE_ARRAY:
            // TODO
            printf("<ARRAY>");
            break;
        case FL_LIB_TYPE_OBJECT:
            // TODO
            printf("<OBJECT>");
            break;
        }
    }

    return FL_NULL;
}

fl_void fl_std_io_register(fl_context *context)
{
}