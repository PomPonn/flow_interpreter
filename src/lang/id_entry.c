#include "id_entry.h"

#include "lang/function.h"

fl_void fl_id_entry_free(fl_id_entry *entry)
{
    // free the 'name' field?

    switch (entry->type)
    {
    case FL_ID_VALUE_TYPE_FUNC:
        fl_func_entry_free((fl_func_entry *)entry);
        break;
    default:
        assert(FL_FALSE);
        break;
    }
}