#include "meta.h"

// ---- INCLUDES ----

#include "macros/modules.h"

// ---- MACROS ----

#define FL_MODULE_NAME meta

// ---- DECLARATIONS ----

// arithmetics
FL_MODULE_FN_HEAD(add, int);
FL_MODULE_FN_HEAD(add, float);

// basic memory operations
FL_MODULE_FN_HEAD_ONCE(assign);

// compile time id's
FL_MODULE_FN_HEAD_ONCE(import);

// ---- DEFINITIONS ----

const fl_context *context = FL_NULL;

fl_id_entry *fl_module_register(fl_context *FL_PARAM_CNT, fl_id_chain *FL_PARAM_ID_CHAIN)
{
    context = FL_PARAM_CNT;

    FL_REGISTER_SCOPE(&cnt->main_scope, FL_MODULE_NAME);

    if (FL_PARAM_ID_CHAIN->size >= 1)
    {
        // import top level ids

        FL_REGISTER_FN(FL_MODULE_NAME, import);
        FL_OVERLOAD_FN_ONCE_CONST(
            import, FL_DATA_TYPE_SCOPE,
            fl_value_desc_create(FL_DATA_TYPE_STRING, FL_FALSE));

        FL_REGISTER_FN(FL_MODULE_NAME, assign);
        FL_OVERLOAD_FN_ONCE(
            assign, FL_DATA_TYPE_ANY,
            fl_value_desc_create(FL_DATA_TYPE_ANY, FL_TRUE),
            fl_value_desc_create(FL_DATA_TYPE_ANY, FL_FALSE));

        FL_REGISTER_FN(FL_MODULE_NAME, add);
        FL_OVERLOAD_FN(
            add, int, FL_DATA_TYPE_INT,
            fl_value_desc_create(FL_DATA_TYPE_INT, FL_FALSE),
            fl_value_desc_create(FL_DATA_TYPE_INT, FL_FALSE));
        FL_OVERLOAD_FN(
            add, float, FL_DATA_TYPE_FLOAT,
            fl_value_desc_create(FL_DATA_TYPE_FLOAT, FL_FALSE),
            fl_value_desc_create(FL_DATA_TYPE_FLOAT, FL_FALSE));
    }

    return fl_scope_resolve_id(&FL_PARAM_CNT->main_scope, FL_PARAM_ID_CHAIN);
}

FL_MODULE_FN_HEAD(add, int)
{
    fl_lang_type_int *res = fl_mem_alloc(sizeof(fl_lang_type_int));

    fl_lang_type_int a = *(fl_lang_type_int *)args.data[0].data;
    fl_lang_type_int b = *(fl_lang_type_int *)args.data[1].data;
    *res = a + b;

    return fl_value_create(FL_DATA_TYPE_INT, res);
}

FL_MODULE_FN_HEAD(add, float)
{
    fl_lang_type_float *res = fl_mem_alloc(sizeof(fl_lang_type_float));

    fl_lang_type_float a = *(fl_lang_type_float *)args.data[0].data;
    fl_lang_type_float b = *(fl_lang_type_float *)args.data[1].data;
    *res = a + b;

    return fl_value_create(FL_DATA_TYPE_INT, res);
}

FL_MODULE_FN_HEAD_ONCE(assign)
{
    return FL_VALUE_EMPTY;
}

FL_MODULE_FN_HEAD_ONCE(import)
{
    return FL_VALUE_EMPTY;
}