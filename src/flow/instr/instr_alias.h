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
#include "flow/global.h"

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

fl_void fl_g_instr_aliases_init();

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

FL_HEADER_FN const fl_fn_interface *fl_instr_alias_get_interface(const fl_instr_alias *instr_alias)
{
    return fl_global_get_static_instr_alias_interface(instr_alias);
}