#include "id_entry.h"

#include "struct/call/callable_entry.h"

fl_void fl_id_entry_free(fl_id_entry *entry)
{
    // free the 'name' field?

    switch (entry->type)
    {
    case FL_ID_ENTRY_TYPE_CALLABLE:
        fl_callable_entry_free((fl_callable_entry *)entry);
        break;
    case FL_ID_ENTRY_TYPE_OBJ:
    default:
        assert(FL_FALSE);
        break;
    }
}