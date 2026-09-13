/**
 * ---------- id_entry.h ----------------
 *
 * Declares id entry logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/data_type.h"
#include "flow/value.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_id_entry
{
    fl_char *name;
    fl_data_type decl_type;
    fl_value value;
};
typedef struct _fl_id_entry fl_id_entry;

// ---------- DECLARATIONS --------------

FL_HEADER_FN fl_void fl_id_entry_free(fl_id_entry **entry);

FL_HEADER_FN fl_void fl_id_entry_init(
    fl_id_entry **entry,
    fl_char *name,
    fl_data_type declared_type,
    fl_value value);

// ---------- DEFINITIONS ---------------

FL_HEADER_FN fl_void fl_id_entry_free(fl_id_entry **entry)
{
    fl_id_entry *ptr = *entry;

    fl_mem_free(ptr->name);
    fl_value_free(&ptr->value);

    fl_mem_free(ptr);
    *entry = FL_NULL;
}

FL_HEADER_FN fl_void fl_id_entry_init(
    fl_id_entry **entry,
    fl_char *name,
    fl_data_type declared_type,
    fl_value value)
{
    fl_id_entry *val = fl_mem_alloc_t(fl_id_entry);
    val->name = name;
    val->decl_type = declared_type;
    val->value = value;

    *entry = val;
}