#include "io.h"

// ---------- INCLUDES ----------

#include "macros/modules.h"
#include "flow/value.h"
#include "flow/scope.h"
#include "flow/id/id_chain.h"
#include <stdio.h>

// ---------- MACROS ----------

#define FL_MODULE_NAME io

// ---------- DECLARATIONS ----------

FL_MODULE_FN_HEAD_ONCE(put);

// ---------- DEFINITIONS ----------

fl_value fl_std_io_register(fl_scope *scope, const fl_id_chain *id_chain)
{
    if (id_chain->size < 2)
        return FL_VALUE_EMPTY;

    if (id_chain->size == 2)
    {
        // return this module as a scope
        fl_scope *io_scope;
        fl_scope_init(&io_scope, scope);

        FL_REGISTER_FN(put, io_scope, FL_FALSE);
        FL_OVERLOAD_FN_ONCE(
            put, FL_DATA_TYPE_NULL,
            fl_data_desc_create(FL_DATA_TYPE_ANY));

        return fl_value_create(FL_DATA_TYPE_SCOPE, io_scope);
    }

    // TODO ...

    assert(FL_FALSE);

    return FL_VALUE_EMPTY;
}

FL_MODULE_FN_HEAD_ONCE(put)
{
    for (fl_size i = 0; i < args.size; i++)
    {
        fl_value arg = args.data[i];
        switch (arg.data_type)
        {
        case FL_DATA_TYPE_NULL:
            printf("<NULL>");
            break;
        case FL_DATA_TYPE_STRING:
            puts(arg.data);
            break;
        case FL_DATA_TYPE_INT:
            printf("%lli\n", *(fl_long *)arg.data);
            break;
        case FL_DATA_TYPE_BOOL:
            printf("%u\n", *(fl_uint *)arg.data);
            break;
        case FL_DATA_TYPE_FLOAT:
            printf("%f\n", *(fl_double *)arg.data);
            break;
        case FL_DATA_TYPE_ARRAY:
            // TODO
            puts("<ARRAY>");
            break;
        case FL_DATA_TYPE_SCOPE:
            // TODO
            puts("<OBJECT>");
            break;
        }
    }

    return FL_VALUE_EMPTY;
}
