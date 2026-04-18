/**
 * ---------- predef.h ----------
 *
 * Declares predefine logic.
 *
 */

#pragma once

// ---------- DEPENDENCIES ----------

#include "shared/common.h"
#include "vecs/value_vec.h"
#include "vecs/param_vec.h"
#include "flow/instr/instruction.h"
#include "flow/fn/fn_interface.h"
#include "flow/globals.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_context);

struct _fl_predef
{
    fl_instruction instr;
    fl_bool escaped;
    fl_char *alias;
};
typedef struct _fl_predef fl_predef;

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_predef fl_predef_create(
    fl_instruction instr,
    fl_char *alias,
    fl_bool escaped)
{
    return (fl_predef){.instr = instr, .alias = alias, .escaped = escaped};
}

FL_HEADER_FN fl_void fl_predef_free(fl_predef *predef)
{
    fl_mem_free(predef->alias);
}

FL_HEADER_FN fl_bool fl_predef_is_inlineable(const fl_predef *predef)
{
    switch (predef->instr)
    {
    case FL_INSTR_IMPORT:
    case FL_INSTR_ASSIGN:
        return FL_FALSE;
    default:
        return FL_TRUE;
    }
}

FL_HEADER_FN const fl_fn_interface *fl_predef_get_interface(const fl_predef *predef)
{
    return fl_globals_get_static_predef_interface(predef);
}

fl_value fl_predef_call(const fl_predef *predef, fl_context *cnt, fl_value_vec args);