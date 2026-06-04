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
#include "context/base_context.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_operation_type
{
    FL_OPERATION_UNINIT,
    FL_OPERATION_INSTR,
    FL_OPERATION_VALUE,
    FL_OPERATION_FN,
};
typedef fl_byte fl_operation_type;

struct _fl_operation
{
    fl_operation_type type;
    union
    {
        fl_instruction instr;
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

FL_HEADER_FN fl_bool fl_operation_is_instr(fl_operation op, fl_instruction instr);
FL_HEADER_FN fl_bool fl_operation_is_callable(fl_operation op);
FL_HEADER_FN const fl_fn_interface *fl_operation_get_interface(
    fl_operation op,
    const fl_param_vec params);

FL_HEADER_FN fl_value fl_operation_run(fl_operation op, const fl_value_vec args, fl_base_context *cnt);

// ---------- DEFINITIONS ----------

FL_HEADER_FN fl_operation fl_operation_from_instr(fl_instruction instr)
{
    return (fl_operation){
        .type = FL_OPERATION_INSTR,
        .instr = instr,
    };
}

FL_HEADER_FN fl_operation fl_operation_from_fn_vars(fl_fn_variant_vec *vars)
{
    return (fl_operation){
        .type = FL_OPERATION_FN,
        .vars = vars,
    };
}

FL_HEADER_FN fl_operation fl_operation_from_value(fl_value val)
{
    return (fl_operation){
        .type = FL_OPERATION_VALUE,
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

FL_HEADER_FN fl_bool fl_operation_is_instr(fl_operation op, fl_instruction instr)
{
    return op.type == FL_OPERATION_INSTR && op.instr == instr;
}

FL_HEADER_FN fl_bool fl_operation_is_callable(fl_operation op)
{
    return op.type == FL_OPERATION_FN ||
           (op.type == FL_OPERATION_INSTR && fl_instruction_is_callable(op.instr));
}

FL_HEADER_FN fl_value fl_operation_run(fl_operation op, const fl_value_vec args, fl_base_context *cnt)
{
    if (op.type == FL_OPERATION_INSTR)
        return fl_instruction_call(op.instr, cnt, args);
    else if (op.type == FL_OPERATION_FN)
    {
        fl_fn_variant *var = fl_fn_variant_vec_find_match_a(op.vars, args);
        return fl_fn_variant_call(var, args);
    }
    else
        return FL_VALUE_EMPTY;
}

FL_HEADER_FN const fl_fn_interface *fl_operation_get_interface(
    fl_operation op,
    const fl_param_vec params)
{
    if (op.type == FL_OPERATION_INSTR)
    {
        const fl_fn_interface *fn_i = fl_instruction_get_interface(op.instr);
        return fl_param_vec_match_params(&fn_i->params, &params) ? fn_i : FL_NULL;
    }
    else if (op.type == FL_OPERATION_FN)
        return &fl_fn_variant_vec_find_match_p(op.vars, params)->interface;
    else
        return FL_NULL;
}