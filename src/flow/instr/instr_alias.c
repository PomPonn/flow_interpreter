/**
 * ---------- instr_alias.c ----------------
 *
 * Implements instr_alias logic.
 *
 * ----------------------------------
 */

#include "instr_alias.h"
#include "vecs/fn_interface_vec.h"
#include "vecs/instr_alias_vec.h"

// ---------- DEPENDENCIES ----------

// ...

// ---------- MACROS ----------------

// ...

// ---------- GLOBALS ---------------

fl_bool g_initialized = FL_FALSE;
fl_fn_interface_vec g_interfaces;
fl_instr_alias_vec g_aliases;

// ---------- DECLARATIONS ----------

// ...

// ---------- DEFINITIONS -----------

fl_void fl_g_instr_aliases_init()
{
    if (g_initialized)
        return;

    fl_fn_interface_vec_initc(&g_interfaces, FL_CLL_INSTR_COUNT);
    fl_instr_alias_vec_initc(&g_aliases, FL_CLL_INSTR_COUNT);
    fl_param_vec params;

    // IMPORT
    fl_param_vec_initc(&params, 1);
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_STRING));
    fl_fn_interface_init(&g_interfaces.data[FL_INSTR_IMPORT], FL_DATA_TYPE_ANY, params);
    fl_instr_alias_vec_push(&g_aliases, fl_instr_alias_create(FL_INSTR_IMPORT, "import"));

    // ASSIGN
    fl_param_vec_initc(&params, 2);
    fl_param_vec_push(&params, fl_param_create_ref(FL_DATA_TYPE_ANY));
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_fn_interface_init(&g_interfaces.data[FL_INSTR_ASSIGN], FL_DATA_TYPE_ANY, params);
    fl_instr_alias_vec_push(&g_aliases, fl_predef_create(FL_INSTR_ASSIGN, fl_str_dup("="), FL_TRUE));

    // ADD
    fl_param_vec_initc(&params, 2);
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_param_vec_push(&params, fl_param_create(FL_DATA_TYPE_ANY));
    fl_fn_interface_init(&g_interfaces.data[FL_INSTR_ADD], FL_DATA_TYPE_INT & FL_DATA_TYPE_FLOAT, params);
    fl_instr_alias_vec_push(&g_aliases, fl_predef_create(FL_INSTR_ADD, fl_str_dup("+"), FL_TRUE));
}