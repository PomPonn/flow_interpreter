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
#include "context/base_context.h"

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
        fl_fn_variant_vec *vars;
        fl_value value;
    };
};
typedef struct _fl_operation fl_operation;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_operation fl_operation_from_instr(fl_instruction instr);
FL_HEADER_FN fl_operation fl_operation_from_fn_vars(fl_fn_variant_vec *vars);
FL_HEADER_FN fl_operation fl_operation_from_value(fl_value val);
FL_HEADER_FN fl_operation fl_operation_uninitialized();
FL_HEADER_FN fl_operation fl_operation_unknown();

FL_HEADER_FN fl_bool fl_operation_is_initialized(fl_operation op);
FL_HEADER_FN fl_bool fl_operation_is_instr(fl_operation op, fl_instruction instr);
FL_HEADER_FN fl_bool fl_operation_is_callable(fl_operation op);
FL_HEADER_FN fl_bool fl_operation_is_const_assign(fl_operation op, fl_param right);
FL_HEADER_FN const fl_fn_interface *fl_operation_get_interface_p(
    fl_operation op,
    const fl_param_vec params);

FL_HEADER_FN fl_value fl_operation_run(fl_operation op, fl_value_vec args, fl_base_context *cnt);

// ---------- DEFINITIONS ----------

FL_HEADER_FN fl_operation fl_operation_from_instr(fl_instruction instr)
{
    return (fl_operation){
        .type = FL_OPERATION_INSTR,
        .instr = instr,
        .vars = fl_g_instrs_get_variants_of(instr),
    };
}

FL_HEADER_FN fl_operation fl_operation_from_fn_vars(fl_fn_variant_vec *vars)
{
    return (fl_operation){
        .type = FL_OPERATION_FN,
        .instr = FL_INSTR_NONE,
        .vars = vars,
    };
}

FL_HEADER_FN fl_operation fl_operation_from_value(fl_value val)
{
    return (fl_operation){
        .type = FL_OPERATION_VALUE,
        .instr = FL_INSTR_NONE,
        .value = val,
    };
}

FL_HEADER_FN fl_operation fl_operation_uninitialized()
{
    return (fl_operation){
        .type = FL_OPERATION_UNINIT,
        .instr = 0,
    };
}

FL_HEADER_FN fl_operation fl_operation_unknown()
{
    return (fl_operation){
        .type = FL_OPERATION_UNKNOWN,
        .instr = 0,
    };
}

FL_HEADER_FN fl_bool fl_operation_is_initialized(fl_operation op)
{
    return op.type != FL_OPERATION_UNINIT;
}

FL_HEADER_FN fl_bool fl_operation_is_instr(fl_operation op, fl_instruction instr)
{
    return op.type == FL_OPERATION_INSTR && op.instr == instr;
}

FL_HEADER_FN fl_bool fl_operation_is_callable(fl_operation op)
{
    return op.type == FL_OPERATION_FN ||
           (op.type == FL_OPERATION_INSTR && fl_instruction_is_callable(op.instr));
}

FL_HEADER_FN fl_value fl_operation_run(fl_operation op, fl_value_vec args, fl_base_context *cnt)
{
    if (!fl_operation_is_callable(op))
        return FL_VALUE_EMPTY;

    fl_fn_variant *var = fl_fn_variant_vec_find_suit(op.vars, &args);

    if (!var)
        assert(FL_FALSE);

    return fl_fn_variant_call(var, args);
}

FL_HEADER_FN const fl_fn_interface *fl_operation_get_interface_p(
    fl_operation op,
    const fl_param_vec params)
{
    if (!fl_operation_is_callable(op))
        return FL_NULL;
    else
    {
        fl_fn_variant *match = fl_fn_variant_vec_find_match(op.vars, params);
        return match ? &match->interface : FL_NULL;
    }
}

FL_HEADER_FN fl_bool fl_operation_is_const_assign(fl_operation op, fl_param right)
{
    return fl_operation_is_instr(op, FL_INSTR_ASSIGN) && fl_bitfield_is_set(right.traits, FL_PARAM_TRAITS_CONST);
}