#pragma once

#include "lang/data_type.h"
#include "lang/value.h"

struct _fl_id_entry
{
    fl_char *name;
    fl_data_type decl_type;
    fl_value value;
};
typedef struct _fl_id_entry fl_id_entry;

FL_HEADER_FN fl_void fl_id_entry_free(fl_id_entry **entry);
FL_HEADER_FN fl_void fl_id_entry_init(
    fl_id_entry **entry,
    fl_char *name,
    fl_data_type declared_type,
    fl_value value);

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
    fl_id_entry *val = fl_mem_alloc(sizeof(fl_id_entry));
    val->name = name;
    val->decl_type = declared_type;
    val->value = value;

    *entry = val;
}