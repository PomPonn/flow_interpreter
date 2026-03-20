#pragma once

// ---- DEPENDENCIES ----

#include "struct/id/id_entry.h"
#include "struct/call/call_param_vec.h"

// ---- DEFINES ----

#define FL_EXTEND_CALLABLE_ID_ENTRY \
    FL_EXTEND_ID_ENTRY;             \
    fl_data_type return_type;       \
    fl_call_param_vec params

// ---- TYPE DEFINITIONS ----

struct _fl_callable_id_entry
{
    FL_EXTEND_CALLABLE_ID_ENTRY;
};
/**
 * @brief Abstract callable id entry;
 * contains information about callable, but without a body.
 */
typedef struct _fl_callable_id_entry fl_callable_id_entry;

FL_HEADER_FN fl_void fl_callable_id_entry_fill(
    fl_callable_id_entry *entry,
    const fl_char *name,
    fl_id_entry_type type,
    fl_bool constant,
    fl_data_type return_type,
    fl_call_param_vec params)
{
    fl_id_entry_fill((fl_id_entry *)entry, name, type, constant);
    entry->return_type = return_type;
    entry->params = params;
}