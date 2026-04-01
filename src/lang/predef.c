#include "predef.h"

#include "vecs/value_vec.h"
#include "lang/instructions.h"

#define _FL_PARAM(type, refable) \
    fl_value_desc_vec_push(&params, fl_value_desc_create(type, refable))

// predefine type params cache
fl_value_desc_vec _client_params_per_type[FL_PREDEF_TYPES_COUNT];

fl_void fl_predef_free_internal()
{
    for (fl_size i = 0; i < FL_PREDEF_TYPES_COUNT; i++)
        fl_value_desc_vec_free(&_client_params_per_type[i]);
}

fl_void fl_predef_init_internal()
{
    fl_value_desc_vec params;

    // IMPORT
    fl_value_desc_vec_initc(&params, 1);
    _FL_PARAM(FL_DATA_TYPE_STRING, FL_FALSE);
    _client_params_per_type[FL_PREDEF_TYPE_IMPORT] = params;

    // ASSIGN
    fl_value_desc_vec_initc(&params, 2);
    _FL_PARAM(FL_DATA_TYPE_ANY, FL_TRUE);
    _FL_PARAM(FL_DATA_TYPE_ANY, FL_FALSE);
    _client_params_per_type[FL_PREDEF_TYPE_ASSIGN] = params;

    // ADD
    fl_value_desc_vec_initc(&params, 2);
    _FL_PARAM(FL_DATA_TYPE_ANY, FL_FALSE);
    _FL_PARAM(FL_DATA_TYPE_ANY, FL_FALSE);
    _client_params_per_type[FL_PREDEF_TYPE_ADD] = params;
}

fl_value_desc_vec fl_predef_get_client_params(const fl_predef *predef)
{
    return _client_params_per_type[predef->type];
}

fl_value fl_predef_call(const fl_predef *predef, fl_value_vec *args)
{
    switch (predef->type)
    {
    case FL_PREDEF_TYPE_ADD:
        return fl_instr_add(args->data[0], args->data[1]);
    case FL_PREDEF_TYPE_ASSIGN:
        return fl_instr_assign(args->data[0], args->data[1]);
    default:
        assert(FL_FALSE);
        return FL_VALUE_EMPTY;
    }
}