/**
 * ---------- g_instrs.c ----------------
 *
 * Implements global instructions logic.
 *
 * ----------------------------------
 */

#include "g_instrs.h"

// ---------- DEPENDENCIES ----------

#include "vecs/fn_interface_vec.h"
#include "flow/instr/instruction.h"
#include "vecs/const_cstr_vec.h"
#include "vecs/instr_alias_vec.h"
#include "vecs/fn_traits.h"

// ---------- GLOBALS ---------------

fl_bool g_initialized = FL_FALSE;
fl_fn_interface_vec g_interfaces;
fl_instr_alias_vec g_aliases;
fl_const_cstr_vec g_aliases_escapes;

// ---------- DECLARATIONS ----------

// ...

// ---------- DEFINITIONS -----------

fl_void fl_g_instrs_free()
{
    fl_fn_interface_vec_free(&g_interfaces);
    fl_instr_alias_vec_free(&g_aliases);
    fl_const_cstr_vec_free(&g_aliases_escapes);

    g_initialized = FL_FALSE;
}

fl_void fl_g_instrs_init()
{
    if (g_initialized)
        return;

    fl_fn_interface_vec_init_s(&g_interfaces, FL_CLL_INSTR_COUNT);
    fl_instr_alias_vec_init_s(&g_aliases, FL_CLL_INSTR_COUNT);
    fl_param_vec params;

    // IMPORT
    fl_param_vec_init_with(
        &params, 1,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_STRING),
        });
    fl_fn_interface_init(&g_interfaces.data[FL_INSTR_IMPORT], FL_DATA_TYPE_ANY, params);
    fl_fn_interface_add_trait(&g_interfaces.data[FL_INSTR_IMPORT], FL_FN_TRAITS_SIMULATE);
    g_aliases.data[FL_INSTR_IMPORT] = fl_instr_alias_create(FL_INSTR_IMPORT, "import");

    // DECLARE
    fl_param_vec_init_with(
        &params, 2,
        (fl_param[]){
            fl_param_create_traited(FL_DATA_TYPE_NULL, FL_PARAM_TRAITS_REF),
            fl_param_create(FL_DATA_TYPE_TYPE),
        });
    fl_fn_interface_init(&g_interfaces.data[FL_INSTR_DECLARE], FL_DATA_TYPE_ANY, params);
    fl_fn_interface_add_trait(&g_interfaces.data[FL_INSTR_DECLARE], FL_FN_TRAITS_SIMULATE);
    g_aliases.data[FL_INSTR_DECLARE] = fl_instr_alias_create_esc(FL_INSTR_DECLARE, ":");

    // ASSIGN
    fl_param_vec_init_with(
        &params, 2,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_ANY),
            fl_param_create(FL_DATA_TYPE_ANY),
        });
    fl_fn_interface_init(&g_interfaces.data[FL_INSTR_ASSIGN], FL_DATA_TYPE_ANY, params);
    fl_fn_interface_add_trait(&g_interfaces.data[FL_INSTR_ASSIGN], FL_FN_TRAITS_SIMULATE);
    g_aliases.data[FL_INSTR_ASSIGN] = fl_instr_alias_create_esc(FL_INSTR_ASSIGN, "=");

    // ADD
    fl_param_vec_init_with(
        &params, 2,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_ANY),
            fl_param_create(FL_DATA_TYPE_ANY),
        });
    fl_fn_interface_init(&g_interfaces.data[FL_INSTR_ADD], FL_DATA_TYPE_ANY, params);
    fl_fn_interface_add_trait(&g_interfaces.data[FL_INSTR_ADD], FL_FN_TRAITS_PURE);
    g_aliases.data[FL_INSTR_ADD] = fl_instr_alias_create_esc(FL_INSTR_ADD, "+");

    g_aliases_escapes = fl_instr_alias_vec_to_escapes(&g_aliases);

    g_initialized = FL_TRUE;
}

const fl_const_cstr_vec *fl_g_instrs_aliases_to_escapes()
{
    return &g_aliases_escapes;
}

const fl_fn_interface *fl_g_instrs_get_interface(fl_instruction instr)
{
    return &g_interfaces.data[instr];
}

const fl_instr_alias *fl_g_instrs_find_alias(const fl_char *alias)
{
    return fl_instr_alias_vec_find(&g_aliases, alias);
}