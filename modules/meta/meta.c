#include "meta.h"

// ---- INCLUDES ----

#include "../_framework.h"
#include "struct/call/call_param_vec.h"
#include "struct/call/callable_id_entry.h"

#define FL_MODULE meta

// ---- DECLARATIONS ----

FL_MODULE_CALLABLE(add);
FL_MODULE_CALLABLE(substract);
FL_MODULE_CALLABLE(multiply);
FL_MODULE_CALLABLE(divide);
FL_MODULE_CALLABLE(power);
FL_MODULE_CALLABLE(import);

// ---- DEFINITIONS ----

fl_id_node *fl_module_register(fl_context *FL_PARAM_CNT, fl_id_chain *FL_PARAM_ID_CHAIN)
{
    if (!FL_PARAM_ID_CHAIN->size)
        return FL_NULL;

    FL_REGISTER_START;

    if (FL_PARAM_ID_CHAIN->size == 1)
    {
        // import top level ids
        FL_REGISTER_CALLABLE_RT(meta, add, FL_DATA_TYPE_INT, FL_DATA_TYPES(FL_DATA_TYPE_ARRAY));
        FL_REGISTER_CALLABLE_CT(meta, import, FL_DATA_TYPE_NULL, FL_DATA_TYPES(FL_DATA_TYPE_ARRAY));
    }

    return fl_id_node_seek(FL_PARAM_CNT->id_tree, FL_PARAM_ID_CHAIN);
}

FL_MODULE_CALLABLE(add)
{
    return FL_NULL;
}

FL_MODULE_CALLABLE(substract)
{
    return FL_NULL;
}

FL_MODULE_CALLABLE(multiply)
{
    return FL_NULL;
}

FL_MODULE_CALLABLE(divide)
{
    return FL_NULL;
}

FL_MODULE_CALLABLE(power)
{
    return FL_NULL;
}

FL_MODULE_CALLABLE(import)
{
    return FL_NULL;
}