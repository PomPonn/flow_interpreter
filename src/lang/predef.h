/**
 * ---- predef.h ----
 *
 * Declares predefine logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "shared/common.h"
#include "vecs/value_vec.h"
#include "vecs/value_desc_vec.h"

// REMEMBER TO KEEP IT UP-TO-DATE
#define FL_PREDEF_TYPES_COUNT FL_PREDEF_TYPE_POW

// ---- TYPE DEFINITIONS ----

// must match start of _fl_cmp_instruction enum from lang/instructions.h
// when add new predefs before FL_PREDEF_TYPE_POW to avoid dependency problems
enum _fl_predef_type
{
    FL_PREDEF_TYPE_IMPORT,
    FL_PREDEF_TYPE_ASSIGN,
    FL_PREDEF_TYPE_ADD,
    FL_PREDEF_TYPE_SUB,
    FL_PREDEF_TYPE_DIV,
    FL_PREDEF_TYPE_MUL,
    FL_PREDEF_TYPE_MOD,
    FL_PREDEF_TYPE_POW,
};
typedef fl_byte fl_predef_type;

struct _fl_predef
{
    fl_predef_type type;
    fl_bool escaped;
    fl_char *alias;
};
typedef struct _fl_predef fl_predef;

// ---- DECLARATIONS ----

FL_HEADER_FN fl_predef fl_predef_create(
    fl_predef_type type,
    fl_char *alias,
    fl_bool escaped)
{
    return (fl_predef){.type = type, .alias = alias, .escaped = escaped};
}

FL_HEADER_FN fl_void fl_predef_free(fl_predef *predef)
{
    fl_mem_free(predef->alias);
}

fl_void fl_predef_free_internal();

fl_void fl_predef_init_internal();

fl_value_desc_vec fl_predef_get_client_params(const fl_predef *predef);

fl_value fl_predef_call(const fl_predef *predef, fl_value_vec *args);