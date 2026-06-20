/**
 * ---------- g_instrs.c ------------------
 *
 * Implements global instructions logic.
 *
 * ----------------------------------------
 */

#include "g_instrs.h"

// ---------- DEPENDENCIES ----------

#include "flow/instr/internal/_all.h"
#include "flow/instr/instruction.h"
#include "vecs/const_cstr_vec.h"
#include "vecs/instr_alias_vec.h"
#include "vecs/fn_variant_vec_vec.h"
#include "shared/str.h"

// ---------- GLOBALS ---------------

fl_bool g_initialized = FL_FALSE;
fl_fn_variant_vec_vec g_variants;
fl_instr_alias_vec g_aliases;
fl_const_cstr_vec g_escapes;

// ---------- DEFINITIONS -----------

const fl_const_cstr_vec *fl_g_instrs_get_escapes()
{
    return &g_escapes;
}

fl_fn_variant_vec *fl_g_instrs_get_variants_of(fl_instruction instr)
{
    return &g_variants.data[instr];
}

fl_instruction fl_g_instrs_resolve_alias(const fl_char *alias)
{
    for (fl_size i = 0; i < g_aliases.size; i++)
        if (fl_str_equal(g_aliases.data[i].alias, alias))
            return (fl_instruction)i;

    return FL_INSTR_NONE;
}

fl_void fl_g_instrs_free()
{
    fl_fn_variant_vec_vec_free(&g_variants);
    fl_instr_alias_vec_free(&g_aliases);
    fl_const_cstr_vec_free(&g_escapes);

    g_initialized = FL_FALSE;
}

fl_void fl_g_instrs_init()
{
    if (g_initialized)
        return;

    fl_fn_variant_vec_vec_init_s(&g_variants, FL_CLL_INSTR_COUNT);
    fl_instr_alias_vec_init_s(&g_aliases, FL_CLL_INSTR_COUNT);
    fl_param_vec params;
    fl_fn_variant_vec vars;
    fl_fn_variant var;
    fl_fn_interface intf;

    // ---- IMPORT ----

    // init params
    fl_param_vec_init_with(
        &params, 1,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_STRING),
        });
    // init interface(s)
    fl_fn_interface_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bitfield_set(intf.traits, FL_FN_TRAITS_META);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf);
    var.pointer = fl_instr_import;
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_IMPORT] = vars;
    g_aliases.data[FL_INSTR_IMPORT] = fl_instr_alias_create("import");

    // ---- ASSIGN ----

    // init params
    fl_param_vec_init_with(
        &params, 2,
        (fl_param[]){
            fl_param_create_traited(FL_DATA_TYPE_ANY, FL_PARAM_TRAITS_REF),
            fl_param_create(FL_DATA_TYPE_ANY),
        });
    // init interface(s)
    fl_fn_interface_init(&intf, FL_DATA_TYPE_ANY, params);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf);
    var.pointer = fl_instr_assign;
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_ASSIGN] = vars;
    g_aliases.data[FL_INSTR_ASSIGN] = fl_instr_alias_create_esc("=");

    // ---- LAMBDA ----

    // init params
    fl_param_vec_init_with(
        &params, 2,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_ARRAY | FL_DATA_TYPE_REF),
            fl_param_create(FL_DATA_TYPE_SCOPE),
        });
    // init interface(s)
    fl_fn_interface_init(&intf, FL_DATA_TYPE_FN, params);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf);
    var.pointer = fl_instr_lambda;
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_LAMBDA] = vars;
    g_aliases.data[FL_INSTR_LAMBDA] = fl_instr_alias_create_esc("->");

    // ---- ADD ----

    // init params
    fl_param_vec_init_with(
        &params, 2,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_ANY),
            fl_param_create(FL_DATA_TYPE_ANY),
        });
    // init interface(s)
    fl_fn_interface_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bitfield_set(intf.traits, FL_FN_TRAITS_PURE);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf);
    var.pointer = fl_instr_add;
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_ADD] = vars;
    g_aliases.data[FL_INSTR_ADD] = fl_instr_alias_create_esc("+");

    // ---- SUB ----

    // init params
    fl_param_vec_init_with(
        &params, 2,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_ANY),
            fl_param_create(FL_DATA_TYPE_ANY),
        });
    // init interface(s)
    fl_fn_interface_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bitfield_set(intf.traits, FL_FN_TRAITS_PURE);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf);
    var.pointer = fl_instr_sub;
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_SUB] = vars;
    g_aliases.data[FL_INSTR_SUB] = fl_instr_alias_create_esc("-");

    // ---- MUL ----

    // init params
    fl_param_vec_init_with(
        &params, 2,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_ANY),
            fl_param_create(FL_DATA_TYPE_ANY),
        });
    // init interface(s)
    fl_fn_interface_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bitfield_set(intf.traits, FL_FN_TRAITS_PURE);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf);
    var.pointer = fl_instr_mul;
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_MUL] = vars;
    g_aliases.data[FL_INSTR_MUL] = fl_instr_alias_create_esc("*");

    g_escapes = fl_instr_alias_vec_to_escapes(&g_aliases);
    g_initialized = FL_TRUE;
}