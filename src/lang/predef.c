#include "predef.h"

#include "vecs/value_vec.h"
#include "lang/instructions.h"
#include "lang/context.h"

#define _FL_PARAM(type, refable) \
    fl_value_desc_vec_push(&params, fl_value_desc_create(type, refable))

// predefine type params
fl_value_desc_vec _client_params_per_type[FL_PREDEF_TYPES_COUNT];
// predefine type return types
fl_data_type _return_types_per_type[FL_PREDEF_TYPES_COUNT];

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
    _return_types_per_type[FL_PREDEF_TYPE_IMPORT] = FL_DATA_TYPE_ANY;

    // ASSIGN
    fl_value_desc_vec_initc(&params, 2);
    _FL_PARAM(FL_DATA_TYPE_ANY, FL_TRUE);
    _FL_PARAM(FL_DATA_TYPE_ANY, FL_FALSE);
    _client_params_per_type[FL_PREDEF_TYPE_ASSIGN] = params;
    _return_types_per_type[FL_PREDEF_TYPE_ASSIGN] = FL_DATA_TYPE_ANY;

    // ADD
    fl_value_desc_vec_initc(&params, 2);
    _FL_PARAM(FL_DATA_TYPE_ANY, FL_FALSE);
    _FL_PARAM(FL_DATA_TYPE_ANY, FL_FALSE);
    _client_params_per_type[FL_PREDEF_TYPE_ADD] = params;
    _return_types_per_type[FL_PREDEF_TYPE_ADD] = FL_DATA_TYPE_INT & FL_DATA_TYPE_FLOAT;
}

const fl_value_desc_vec fl_predef_get_client_params(const fl_predef *predef)
{
    return _client_params_per_type[predef->type];
}

fl_data_type fl_predef_get_return_type(const fl_predef *predef)
{
    return _return_types_per_type[predef->type];
}

fl_value fl_predef_call(const fl_predef *predef, fl_context *cnt, fl_value_vec args)
{
    switch (predef->type)
    {
    case FL_PREDEF_TYPE_ADD:
        return fl_instr_add(args.data[0], args.data[1]);
    case FL_PREDEF_TYPE_ASSIGN:
        return fl_instr_assign(args.data[0], args.data[1]);
    case FL_PREDEF_TYPE_IMPORT:
        return fl_instr_import(cnt, args.data[0].data);
    default:
        assert(FL_FALSE);
        return FL_VALUE_EMPTY;
    }
}