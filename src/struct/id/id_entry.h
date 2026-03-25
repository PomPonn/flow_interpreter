#pragma once

#include "lang/data_type.h"
#include "callable_id_data.h"

struct _fl_id_entry
{
    fl_char *name;
    fl_data_type data_type;
    fl_unknown *data;
};
typedef struct _fl_id_entry fl_id_entry;

FL_HEADER_FN fl_data_type fl_id_entry_final_data_type(const fl_id_entry *entry)
{
    return entry->data_type == FL_DATA_TYPE_CALLABLE ? ((fl_callable_id_data *)entry)->return_type
                                                     : entry->data_type;
}

fl_void fl_id_entry_free(fl_id_entry **entry);

FL_HEADER_FN fl_void fl_id_entry_init(
    fl_id_entry **entry,
    fl_char *name,
    fl_data_type data_type,
    fl_unknown *data)
{
    fl_id_entry *val = fl_mem_alloc(sizeof(fl_id_entry));
    val->name = name;
    val->data_type = data_type;
    val->data = data;

    *entry = val;
}