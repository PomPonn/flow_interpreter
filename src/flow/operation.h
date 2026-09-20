/**
 * ---------- operation.h ------------------------
 *
 * Declares operation logic
 * (join of instructions and normal functions).
 *
 * -----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "shared/common.h"
#include "flow/instr/instruction.h"
#include "vecs/fn_variant_vec.h"
#include "vecs/value_vec.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_operation_type
{
    FL_OPERATION_UNINIT,
    FL_OPERATION_UNKNOWN,
    FL_OPERATION_INSTR,
    FL_OPERATION_VALUE,
    FL_OPERATION_FN,
};
typedef fl_byte fl_operation_type;

struct _fl_operation
{
    fl_operation_type type;
    fl_instruction instr;
    union
    {
        const fl_fn_variant_vec *vars;
        fl_value value;
    };
};
typedef struct _fl_operation fl_operation;

// ---------- DECLARATIONS ----------

FL_FORWARD_STRUCT(fl_base_context);
FL_FORWARD_STRUCT(fl_ast_node);
FL_FORWARD_STRUCT(fl_scope);

FL_INLINE_FN fl_operation fl_operation_from_instr(fl_instruction instr);
FL_INLINE_FN fl_operation fl_operation_from_fn_vars(const fl_fn_variant_vec *vars);
FL_INLINE_FN fl_operation fl_operation_from_value(fl_value val);
FL_INLINE_FN fl_operation fl_operation_uninitialized();
FL_INLINE_FN fl_operation fl_operation_unknown();

FL_INLINE_FN fl_bool fl_operation_is_initialized(fl_operation op);
FL_INLINE_FN fl_bool fl_operation_is_instr(fl_operation op, fl_instruction instr);
FL_INLINE_FN fl_bool fl_operation_is_callable(fl_operation op);
FL_INLINE_FN fl_bool fl_operation_is_compile_time(fl_operation op);

fl_operation fl_operation_from_id_node(const fl_ast_node *id_node, const fl_scope *scope);
fl_fn_variant *fl_operation_prep_run(fl_operation op, fl_value_vec *args, fl_base_context *cnt);
fl_void fl_operation_free(fl_operation *op);

const fl_fn_intf *fl_operation_get_intf_by_descs(
    fl_operation op,
    const fl_data_desc_vec *descs);

const fl_fn_intf *fl_operation_get_intf_by_args(
    fl_operation op,
    const fl_value_vec *args);

// ---------- DEFINITIONS ----------

FL_INLINE_FN fl_operation fl_operation_from_instr(fl_instruction instr)
{
    return (fl_operation){
        .type = FL_OPERATION_INSTR,
        .instr = instr,
        .vars = fl_g_instrs_get_variants_of(instr),
    };
}

FL_INLINE_FN fl_operation fl_operation_from_fn_vars(const fl_fn_variant_vec *vars)
{
    return (fl_operation){
        .type = FL_OPERATION_FN,
        .instr = FL_INSTR_NONE,
        .vars = vars,
    };
}

FL_INLINE_FN fl_operation fl_operation_from_value(fl_value val)
{
    return (fl_operation){
        .type = FL_OPERATION_VALUE,
        .instr = FL_INSTR_NONE,
        .value = val,
    };
}

FL_INLINE_FN fl_operation fl_operation_uninitialized()
{
    return (fl_operation){
        .type = FL_OPERATION_UNINIT,
        .instr = 0,
    };
}

FL_INLINE_FN fl_operation fl_operation_unknown()
{
    return (fl_operation){
        .type = FL_OPERATION_UNKNOWN,
        .instr = 0,
    };
}

FL_INLINE_FN fl_bool fl_operation_is_initialized(fl_operation op)
{
    return op.type != FL_OPERATION_UNINIT;
}

FL_INLINE_FN fl_bool fl_operation_is_instr(fl_operation op, fl_instruction instr)
{
    return op.type == FL_OPERATION_INSTR && op.instr == instr;
}

FL_INLINE_FN fl_bool fl_operation_is_callable(fl_operation op)
{
    return op.type == FL_OPERATION_FN ||
           (op.type == FL_OPERATION_INSTR && fl_instruction_is_callable(op.instr));
}

FL_INLINE_FN fl_bool fl_operation_is_compile_time(fl_operation op)
{
    if (op.type != FL_OPERATION_INSTR)
        return FL_FALSE;

    return op.instr == FL_INSTR_LAMBDA;
}
