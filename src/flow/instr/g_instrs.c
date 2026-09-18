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
#include "vecs/instr_info_vec.h"
#include "vecs/fn_variant_vec_vec.h"
#include "shared/str.h"

// ---------- GLOBALS ---------------

fl_bool g_initialized = FL_FALSE;
fl_fn_variant_vec_vec g_variants;
fl_instr_info_vec g_info_vec;
fl_const_cstr_vec g_escapes;

// ---------- DEFINITIONS -----------

const fl_const_cstr_vec *fl_g_instrs_get_escapes()
{
    return &g_escapes;
}

const fl_instr_info_vec *fl_g_instrs_get_info_vec()
{
    return &g_info_vec;
}

const fl_fn_variant_vec *fl_g_instrs_get_variants_of(fl_instruction instr)
{
    return &g_variants.data[instr];
}

fl_instruction fl_g_instrs_resolve_alias(const fl_char *name)
{
    for (fl_size i = 0; i < g_info_vec.size; i++)
        if (fl_str_equal(g_info_vec.data[i].name, name))
            return (fl_instruction)i;

    return FL_INSTR_NONE;
}

fl_void fl_g_instrs_free()
{
    fl_fn_variant_vec_vec_free(&g_variants);
    fl_instr_info_vec_free(&g_info_vec);
    fl_const_cstr_vec_free(&g_escapes);

    g_initialized = FL_FALSE;
}

fl_void fl_g_instrs_init()
{
    if (g_initialized)
        return;

    fl_fn_variant_vec_vec_init_s(&g_variants, FL_CLL_INSTR_COUNT);
    fl_instr_info_vec_init_s(&g_info_vec, FL_CLL_INSTR_COUNT);

    fl_data_desc_vec params;
    fl_fn_variant_vec vars;
    fl_fn_variant var;
    fl_fn_intf intf;

    // ---- IMPORT ----

    // init params
    fl_data_desc_vec_init_with(
        &params, 1,
        (fl_data_desc[]){
            fl_data_desc_create(FL_DATA_TYPE_STRING),
        });
    // init intf(s)
    fl_fn_intf_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bits_set(intf.traits, FL_FN_TRAITS_META);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf, fl_instr_import);
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_IMPORT] = vars;
    g_info_vec.data[FL_INSTR_IMPORT] = fl_instr_info_create("import", FL_OP_ORDER_NEUTRAL);

    // ---- ASSIGN ----

    // init params
    fl_data_desc_vec_init_with(
        &params, 2,
        (fl_data_desc[]){
            fl_data_desc_create_traited(FL_DATA_TYPE_ANY, FL_DATA_TRAIT_REF),
            fl_data_desc_create(FL_DATA_TYPE_ANY),
        });
    // init intf(s)
    fl_fn_intf_init(&intf, FL_DATA_TYPE_ANY, params);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf, fl_instr_assign);
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_ASSIGN] = vars;
    g_info_vec.data[FL_INSTR_ASSIGN] = fl_instr_info_create("=", FL_OP_ORDER_ASSIGN);

    // ---- LAMBDA ----

    // init params
    fl_data_desc_vec_init_with(
        &params, 2,
        (fl_data_desc[]){
            fl_data_desc_create(FL_DATA_TYPE_ARRAY),
            fl_data_desc_create(FL_DATA_TYPE_CODE_SEGM),
        });
    // init intf(s)
    fl_fn_intf_init(&intf, FL_DATA_TYPE_FN, params);
    fl_bits_set(intf.traits, FL_FN_TRAITS_PURE);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf, fl_instr_lambda);
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_LAMBDA] = vars;
    g_info_vec.data[FL_INSTR_LAMBDA] = fl_instr_info_create("->", FL_OP_ORDER_LAMBDA);

    // ---- ADD ----

    // init params
    fl_data_desc_vec_init_with(
        &params, 2,
        (fl_data_desc[]){
            fl_data_desc_create(FL_DATA_TYPE_ANY),
            fl_data_desc_create(FL_DATA_TYPE_ANY),
        });
    // init intf(s)
    fl_fn_intf_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bits_set(intf.traits, FL_FN_TRAITS_PURE);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf, fl_instr_add);
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_ADD] = vars;
    g_info_vec.data[FL_INSTR_ADD] = fl_instr_info_create("+", FL_OP_ORDER_ARITHM_LOW);

    // ---- SUB ----

    // init params
    fl_data_desc_vec_init_with(
        &params, 2,
        (fl_data_desc[]){
            fl_data_desc_create(FL_DATA_TYPE_ANY),
            fl_data_desc_create(FL_DATA_TYPE_ANY),
        });
    // init intf(s)
    fl_fn_intf_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bits_set(intf.traits, FL_FN_TRAITS_PURE);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf, fl_instr_sub);
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_SUB] = vars;
    g_info_vec.data[FL_INSTR_SUB] = fl_instr_info_create("-", FL_OP_ORDER_ARITHM_LOW);

    // ---- MUL ----

    // init params
    fl_data_desc_vec_init_with(
        &params, 2,
        (fl_data_desc[]){
            fl_data_desc_create(FL_DATA_TYPE_ANY),
            fl_data_desc_create(FL_DATA_TYPE_ANY),
        });
    // init intf(s)
    fl_fn_intf_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bits_set(intf.traits, FL_FN_TRAITS_PURE);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf, fl_instr_mul);
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_MUL] = vars;
    g_info_vec.data[FL_INSTR_MUL] = fl_instr_info_create("*", FL_OP_ORDER_ARITHM_MED);

    // ---- DIV ----

    // init params
    fl_data_desc_vec_init_with(
        &params, 2,
        (fl_data_desc[]){
            fl_data_desc_create(FL_DATA_TYPE_ANY),
            fl_data_desc_create(FL_DATA_TYPE_ANY),
        });
    // init intf(s)
    fl_fn_intf_init(&intf, FL_DATA_TYPE_ANY, params);
    fl_bits_set(intf.traits, FL_FN_TRAITS_PURE);
    // init variant(s)
    fl_fn_variant_init(&var, FL_TRUE, intf, fl_instr_div);
    // set variant vec
    fl_fn_variant_vec_init_with(&vars, 1, &var);
    // set globals
    g_variants.data[FL_INSTR_DIV] = vars;
    g_info_vec.data[FL_INSTR_DIV] = fl_instr_info_create("/", FL_OP_ORDER_ARITHM_MED);

    g_escapes = fl_instr_info_vec_to_escapes(&g_info_vec);
    g_initialized = FL_TRUE;
}