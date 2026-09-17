/**
 * ---------- instr_info.h --------------
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
    // Instruction alias.
    const fl_char *name;

    // Whether it shoud be escaped.
    fl_bool escape;

    // Instruction sorting order.
    fl_int order;
};

/**
 * @brief Instruction information.
 */
typedef struct _fl_instr_info fl_instr_info;

// ---------- DECLARATIONS ----------

/**
 * @brief Create instruction information.
 * @param name Name (alias) of instruction.
 * @param order Sorting order of instruction.
 * @return New instruction info.
 */
FL_SEALED_FN fl_instr_info fl_instr_info_create(const fl_char *name, fl_op_order order);

// ---------- DEFINITIONS -----------

FL_SEALED_FN fl_instr_info fl_instr_info_create(const fl_char *name, fl_op_order order)
{
    return (fl_instr_info){
        .name = name,
        .order = order,
        .escape = FL_TRUE,
    };
}