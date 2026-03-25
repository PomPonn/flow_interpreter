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

// compile time id's
FL_MODULE_CALLABLE(import);

// ---- DEFINITIONS ----

fl_id_entry *fl_module_register(fl_context *FL_PARAM_CNT, fl_id_chain *FL_PARAM_ID_CHAIN)
{
    FL_REGISTER_START;

    FL_REGISTER_SCOPE(&cnt->main_scope, FL_MODULE_NAME);

    if (FL_PARAM_ID_CHAIN->size >= 1)
    {
        // import top level ids

        FL_REGISTER_CT_CALLABLE(
            import, FL_DATA_TYPE_SCOPE,
            FL_DATA_TYPE_DESC_DEFAULT(FL_DATA_TYPE_STRING));

        FL_REGISTER_RT_CALLABLE(
            assign, FL_DATA_TYPE_ANY,
            FL_DATA_TYPE_DESC_ASSIGNABLE(FL_DATA_TYPE_ANY),
            FL_DATA_TYPE_DESC_DEFAULT(FL_DATA_TYPE_ANY));

        FL_REGISTER_RT_CALLABLE(
            add, FL_DATA_TYPE_INT,
            FL_DATA_TYPE_DESC_DEFAULT(FL_DATA_TYPE_INT),
            FL_DATA_TYPE_DESC_DEFAULT(FL_DATA_TYPE_INT));
    }

    fl_uint offset;
    fl_int_vec inds = fl_scope_resolve_id(&FL_PARAM_CNT->main_scope, FL_PARAM_ID_CHAIN, &offset);
    fl_id_entry *id = fl_scope_traverse(&FL_PARAM_CNT->main_scope, inds, offset);
    fl_int_vec_free(&inds);
    return id;
}

FL_MODULE_CALLABLE(add)
{
    fl_lang_type_int *res = fl_mem_alloc(sizeof(fl_lang_type_int));

    fl_lang_type_int a = *(fl_lang_type_int *)args.data[0];
    fl_lang_type_int b = *(fl_lang_type_int *)args.data[1];
    *res = a + b;

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
    0 return FL_NULL;
}

FL_MODULE_CALLABLE(import)
{
    return FL_NULL;
}