#include "id_entry.h"

#include "structs/fn/fn_entry.h"
#include "structs/opr/opr_entry.h"

fl_void fl_id_entry_free(fl_id_entry *entry)
{
    // free the 'name' field?

    switch (entry->type)
    {
    case FL_ID_ENTRY_TYPE_FUNC:
        fl_fn_entry_free((fl_fn_entry *)entry);
        break;
    case FL_ID_ENTRY_TYPE_OPR:
        fl_opr_entry_free((fl_opr_entry *)entry);
        break;
    case FL_ID_ENTRY_TYPE_OBJ:
    default:
        assert(FL_FALSE);
        break;
    }
}