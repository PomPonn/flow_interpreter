#include "meta.h"

// ---- INCLUDES ----

#include "macros/modules.h"

// ---- DEFINITIONS ----

#define FL_MODULE_NAME meta

// ---- DECLARATIONS ----

// arithmetics
FL_MODULE_FN(add);
FL_MODULE_FN(substract);
FL_MODULE_FN(multiply);
FL_MODULE_FN(divide);
FL_MODULE_FN(power);

// basic memory operations
FL_MODULE_FN(assign);

// compile time id's
FL_MODULE_FN(import);

// ---- DEFINITIONS ----

fl_id_entry *fl_module_register(fl_context *FL_PARAM_CNT, fl_id_chain *FL_PARAM_ID_CHAIN)
{
    FL_REGISTER_START;

    FL_REGISTER_SCOPE(&cnt->main_scope, FL_MODULE_NAME);

    if (FL_PARAM_ID_CHAIN->size >= 1)
    {
        // import top level ids

        FL_REGISTER_CT_FN(
            import, FL_DATA_TYPE_SCOPE,
            fl_value_desc_create(FL_DATA_TYPE_STRING, FL_FALSE));

        FL_REGISTER_RT_FN(
            assign, FL_DATA_TYPE_ANY,
            fl_value_desc_create(FL_DATA_TYPE_ANY, FL_TRUE),
            fl_value_desc_create(FL_DATA_TYPE_ANY, FL_FALSE));

        FL_REGISTER_RT_FN(
            add, FL_DATA_TYPE_INT,
            fl_value_desc_create(FL_DATA_TYPE_INT, FL_FALSE),
            fl_value_desc_create(FL_DATA_TYPE_INT, FL_FALSE));
    }

    fl_uint offset;
    fl_int_vec inds = fl_scope_resolve_id(&FL_PARAM_CNT->main_scope, FL_PARAM_ID_CHAIN, &offset);
    fl_id_entry *id = fl_scope_traverse(&FL_PARAM_CNT->main_scope, inds, offset);
    fl_int_vec_free(&inds);
    return id;
}

FL_MODULE_FN(add)
{
    fl_lang_type_int *res = fl_mem_alloc(sizeof(fl_lang_type_int));

    fl_lang_type_int a = *(fl_lang_type_int *)args.data[0];
    fl_lang_type_int b = *(fl_lang_type_int *)args.data[1];
    *res = a + b;

    return res;
}

FL_MODULE_FN(substract)
{
    return FL_NULL;
}

FL_MODULE_FN(multiply)
{
    return FL_NULL;
}

FL_MODULE_FN(divide)
{
    return FL_NULL;
}

FL_MODULE_FN(power)
{
    return FL_NULL;
}

FL_MODULE_FN(assign)
{
    return FL_NULL;
}

FL_MODULE_FN(import)
{
    return FL_NULL;
}