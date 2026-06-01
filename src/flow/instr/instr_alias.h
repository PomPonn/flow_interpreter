/**
 * ---------- instr_alias.h ----------
 *
 * Declares instruction alias logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "shared/common.h"
#include "flow/instr/instruction.h"
#include "flow/fn/fn_interface.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_context);

struct _fl_instr_alias
{
    fl_instruction instr;
    const fl_char *alias;
    fl_bool escape;
};
typedef struct _fl_instr_alias fl_instr_alias;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_instr_alias fl_instr_alias_create(fl_instruction instr, const fl_char *alias);

FL_HEADER_FN fl_instr_alias fl_instr_alias_create_esc(fl_instruction instr, const fl_char *alias);

// ---------- DEFINITIONS -----------

FL_HEADER_FN fl_instr_alias fl_instr_alias_create(fl_instruction instr, const fl_char *alias)
{
    return (fl_instr_alias){
        .instr = instr,
        .alias = alias,
        .escape = FL_FALSE,
    };
}

FL_HEADER_FN fl_instr_alias fl_instr_alias_create_esc(fl_instruction instr, const fl_char *alias)
{
    return (fl_instr_alias){
        .instr = instr,
        .alias = alias,
        .escape = FL_TRUE,
    };
}