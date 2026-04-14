#include "predef.h"

#include "vecs/value_vec.h"
#include "vecs/fn_interface_vec.h"
#include "flow/instr/internal/_all.h"
#include "flow/context.h"

// predefined callable instructions interfaces
fl_fn_interface_vec _static_predefs_interfaces;

fl_void fl_predef_interfaces_free()
{
    fl_fn_interface_vec_free(&_static_predefs_interfaces);
}

fl_void fl_predef_interfaces_init()
{
    fl_fn_interface_vec_initc(&_static_predefs_interfaces, FL_PREDEF_INSTRS_COUNT);
    fl_param_vec params;

    // IMPORT
    fl_param_vec_initc(&params, 1);
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_STRING));
    fl_fn_interface_init(&_static_predefs_interfaces.data[FL_INSTR_IMPORT], FL_DATA_TYPE_ANY, params);

    // ASSIGN
    fl_param_vec_initc(&params, 2);
    fl_param_vec_push(&params, fl_param_create_ref(FL_DATA_TYPE_ANY));
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_fn_interface_init(&_static_predefs_interfaces.data[FL_INSTR_ASSIGN], FL_DATA_TYPE_ANY, params);

    // ADD
    fl_param_vec_initc(&params, 2);
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_fn_interface_init(&_static_predefs_interfaces.data[FL_INSTR_ADD], FL_DATA_TYPE_INT & FL_DATA_TYPE_FLOAT, params);
}

const fl_fn_interface *fl_predef_get_interface(const fl_predef *predef)
{
    return &_static_predefs_interfaces.data[predef->instr];
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