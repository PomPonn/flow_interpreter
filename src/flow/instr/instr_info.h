/**
 * ---------- instr_info.h -------------
 *
 * Declares instruction info logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "flow/instr/instruction.h"
#include "flow/op_order.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_instr_info
{
    const fl_char *name;
    fl_bool escape;
    fl_int order;
};
typedef struct _fl_instr_info fl_instr_info;

// ---------- DECLARATIONS ----------

FL_SEALED_FN fl_instr_info fl_instr_info_create(const fl_char *name);
FL_SEALED_FN fl_instr_info fl_instr_info_create_op(const fl_char *name, fl_op_order order);

// ---------- DEFINITIONS -----------

FL_SEALED_FN fl_instr_info fl_instr_info_create(const fl_char *name)
{
    return (fl_instr_info){
        .name = name,
        .order = FL_OP_ORDER_NEUTRAL,
        .escape = FL_FALSE,
    };
}

FL_SEALED_FN fl_instr_info fl_instr_info_create_op(const fl_char *name, fl_op_order order)
{
    return (fl_instr_info){
        .name = name,
        .order = order,
        .escape = FL_TRUE,
    };
}