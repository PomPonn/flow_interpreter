/**
 * ---------- globals.c ----------------
 *
 * Implements global modules logic.
 *
 * ----------------------------------
 */

#include "globals.h"

// ---------- DEPENDENCIES ----------

#include "vecs/fn_interface_vec.h"
#include "instr/instruction.h"
#include "flow/predef.h"
#include "flow/context.h"

// ---------- MACROS ----------------

// ...

// ---------- GLOBALS ---------------

// predefined callable instructions interfaces
fl_fn_interface_vec _static_predefs_interfaces;

// ---------- DECLARATIONS ----------

// ---------- DEFINITIONS -----------

fl_void fl_globals_init()
{
    // ==== PREDEFS INTERFACES ====

    fl_fn_interface_vec_initc(&_static_predefs_interfaces, FL_INSTR_PREDEF_COUNT);
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

fl_void fl_globals_static_predefs_to_context(fl_context *cnt)
{
    fl_predef_vec_push(&cnt->predefines, fl_predef_create(FL_INSTR_IMPORT, fl_str_dup("import"), FL_FALSE));
    fl_predef_vec_push(&cnt->predefines, fl_predef_create(FL_INSTR_ADD, fl_str_dup("+"), FL_TRUE));
    fl_predef_vec_push(&cnt->predefines, fl_predef_create(FL_INSTR_ASSIGN, fl_str_dup("="), FL_TRUE));
}

fl_void fl_globals_free()
{
    fl_fn_interface_vec_free(&_static_predefs_interfaces);
}

const fl_fn_interface *fl_globals_get_static_predef_interface(const fl_predef *predef)
{
    return &_static_predefs_interfaces.data[predef->instr];
}