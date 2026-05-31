/**
 * ---------- operation.h ---------------------
 *
 * Declares operation logic
 * (join of instructions and normal functions).
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "shared/common.h"
#include "flow/instr/instruction.h"
#include "vecs/fn_variant_vec.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_operation_type
{
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
        fl_value *value;
    };
};
typedef struct _fl_operation fl_operation;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_operation fl_operation_from_instr(fl_instruction instr);
FL_HEADER_FN fl_operation fl_operation_from_fn_vars(fl_fn_variant_vec *vars);
FL_HEADER_FN fl_operation fl_operation_from_value(fl_value *val);

FL_HEADER_FN fl_bool fl_operation_is_instr(fl_operation op, fl_instruction instr);

// ---------- HEADER-ONLY DEFINITIONS ----------

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

FL_HEADER_FN fl_operation fl_operation_from_value(fl_value *val)
{
    return (fl_operation){
        .type = FL_OPERATION_VALUE,
        .value = val,
    };
}

FL_HEADER_FN fl_bool fl_operation_is_instr(fl_operation op, fl_instruction instr)
{
    return op.type == FL_OPERATION_INSTR && op.instr == instr;
}