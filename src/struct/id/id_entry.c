#include "id_entry.h"

#include "struct/id/callable_id_data.h"
#include "struct/id/scope_id_data.h"

fl_void fl_id_entry_free(fl_id_entry **entry)
{
    fl_id_entry *ptr = *entry;

    fl_mem_free(ptr->name);

    switch (ptr->data_type)
    {
    case FL_DATA_TYPE_CALLABLE:
        fl_callable_id_data_free(&(fl_callable_id_data *)ptr->data);
        break;
    case FL_DATA_TYPE_SCOPE:
        fl_scope_id_data_free(&(fl_scope_id_data *)ptr->data);
        break;
    default:
        fl_mem_free(ptr->data);
        break;
    }

    *entry = FL_NULL;
}