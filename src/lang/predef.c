#include "predef.h"

#include "vecs/value_vec.h"
#include "lang/instructions.h"
#include "lang/context.h"

// predefine type params
fl_param_vec _client_params_per_type[FL_PREDEF_INSTRS_COUNT];
// predefine type return types
fl_data_type _return_types_per_type[FL_PREDEF_INSTRS_COUNT];

fl_void fl_static_predefs_free()
{
    for (fl_size i = 0; i < FL_PREDEF_INSTRS_COUNT; i++)
        fl_param_vec_free(&_client_params_per_type[i]);
}

fl_void fl_static_predefs_init()
{
    fl_param_vec params;

    // IMPORT
    fl_param_vec_initc(&params, 1);
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_STRING));
    _client_params_per_type[FL_INSTR_IMPORT] = params;
    _return_types_per_type[FL_INSTR_IMPORT] = FL_DATA_TYPE_ANY;

    // ASSIGN
    fl_param_vec_initc(&params, 2);
    fl_param_vec_push(&params, fl_param_create_ref(FL_DATA_TYPE_ANY));
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    _client_params_per_type[FL_INSTR_ASSIGN] = params;
    _return_types_per_type[FL_INSTR_ASSIGN] = FL_DATA_TYPE_ANY;

    // ADD
    fl_param_vec_initc(&params, 2);
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    _client_params_per_type[FL_INSTR_ADD] = params;
    _return_types_per_type[FL_INSTR_ADD] = FL_DATA_TYPE_INT & FL_DATA_TYPE_FLOAT;
}

const fl_param_vec fl_predef_get_client_params(const fl_predef *predef)
{
    return _client_params_per_type[predef->instr];
}

fl_data_type fl_predef_get_return_type(const fl_predef *predef)
{
    return _return_types_per_type[predef->instr];
}

fl_value fl_predef_call(const fl_predef *predef, fl_context *cnt, fl_value_vec args)
{
    switch (predef->instr)
    {
    case FL_INSTR_ADD:
        return fl_instr_add(args.data[0], args.data[1]);
    case FL_INSTR_ASSIGN:
        return fl_instr_assign(args.data[0], args.data[1]);
    case FL_INSTR_IMPORT:
        return fl_instr_import(cnt, args.data[0].data);
    default:
        assert(FL_FALSE);
        return FL_VALUE_EMPTY;
    }
}