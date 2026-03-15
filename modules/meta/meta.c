#include "meta.h"

// ---- INCLUDES ----

#include "macros/modules.h"

// ---- DEFINITIONS ----

#define FL_MODULE_NAME meta

// ---- DECLARATIONS ----

// arithmetics
FL_MODULE_CALLABLE(add);
FL_MODULE_CALLABLE(substract);
FL_MODULE_CALLABLE(multiply);
FL_MODULE_CALLABLE(divide);
FL_MODULE_CALLABLE(power);

// basic memory operations
FL_MODULE_CALLABLE(assign);

// compile time callables
FL_MODULE_CALLABLE(import);

// ---- DEFINITIONS ----

fl_id_node *fl_module_register(fl_context *FL_PARAM_CNT, fl_id_chain *FL_PARAM_ID_CHAIN)
{
    FL_REGISTER_START;

    FL_REGISTER_OBJ(FL_NULL, FL_MODULE_NAME);

    if (FL_PARAM_ID_CHAIN->size >= 1)
    {
        // import top level ids
        FL_REGISTER_CT_CALLABLE(import, FL_DATA_TYPE_NULL, FL_DATA_TYPES(FL_DATA_TYPE_ARRAY));
        FL_REGISTER_RT_CALLABLE(add, FL_DATA_TYPE_INT, FL_DATA_TYPES(FL_DATA_TYPE_ARRAY));
    }

    return fl_id_node_seek(FL_PARAM_CNT->id_tree, FL_PARAM_ID_CHAIN);
}

FL_MODULE_CALLABLE(add)
{
    fl_lang_type_int *res = fl_mem_alloc(sizeof(fl_lang_type_int));
    return res;
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

FL_MODULE_CALLABLE(assign)
{
    return FL_NULL;
}

FL_MODULE_CALLABLE(import)
{
    return FL_NULL;
}