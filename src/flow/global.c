/**
 * ---------- globals.c ----------------
 *
 * Implements global modules logic.
 *
 * ----------------------------------
 */

#include "global.h"

// ---------- DEPENDENCIES ----------

#include "vecs/fn_interface_vec.h"
#include "instr/instruction.h"
#include "flow/predef.h"
#include "flow/context.h"

// ---------- MACROS ----------------

// ...

// ---------- GLOBALS ---------------

fl_bool initialized = FL_FALSE;
// callable instructions interfaces
fl_fn_interface_vec _instrs_interfaces;
fl_predef_vec _static_predefs;

// ---------- DECLARATIONS ----------

// ---------- DEFINITIONS -----------

fl_void fl_global_init()
{
    if (initialized)
        return;

    // ==== PREDEFS INTERFACES ====

    fl_fn_interface_vec_initc(&_instrs_interfaces, FL_CLL_INSTR_COUNT);
    fl_predef_vec_initc(&_static_predefs, FL_CLL_INSTR_COUNT);
    fl_param_vec params;

    // IMPORT
    fl_param_vec_initc(&params, 1);
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_STRING));
    fl_fn_interface_init(&_instrs_interfaces.data[FL_INSTR_IMPORT], FL_DATA_TYPE_ANY, params);
    fl_predef_vec_push(&_static_predefs, fl_predef_create(FL_INSTR_IMPORT, fl_str_dup("import"), FL_FALSE));

    // ASSIGN
    fl_param_vec_initc(&params, 2);
    fl_param_vec_push(&params, fl_param_create_ref(FL_DATA_TYPE_ANY));
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_fn_interface_init(&_instrs_interfaces.data[FL_INSTR_ASSIGN], FL_DATA_TYPE_ANY, params);
    fl_predef_vec_push(&_static_predefs, fl_predef_create(FL_INSTR_ASSIGN, fl_str_dup("="), FL_TRUE));

    // ADD
    fl_param_vec_initc(&params, 2);
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_fn_interface_init(&_instrs_interfaces.data[FL_INSTR_ADD], FL_DATA_TYPE_INT & FL_DATA_TYPE_FLOAT, params);
    fl_predef_vec_push(&_static_predefs, fl_predef_create(FL_INSTR_ADD, fl_str_dup("+"), FL_TRUE));

    initialized = FL_TRUE;
}

fl_void fl_global_put_static_predefs(fl_context *cnt)
{
    cnt->predefines = fl_predef_vec_copy(&_static_predefs);
}

fl_void fl_global_free()
{
    fl_fn_interface_vec_free(&_instrs_interfaces);
}

// TODO: remove this function
const fl_fn_interface *fl_global_get_static_predef_interface(const fl_predef *predef)
{
    return fl_global_get_instr_interface(predef->instr);
}

fl_const_cstr_vec fl_global_get_static_predefs_escapes()
{
    return fl_predef_vec_escapes_view(&_static_predefs);
}

const fl_fn_interface *fl_global_get_instr_interface(fl_instruction instr)
{
    return &_instrs_interfaces.data[instr];
}