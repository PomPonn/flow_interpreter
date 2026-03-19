#include "id_entry.h"

#include "struct/id/c_callable_id_entry.h"
#include "struct/id/flow_callable_id_entry.h"

fl_void fl_id_entry_free(fl_id_entry **entry)
{
    // free the 'name' field?

    switch ((*entry)->type)
    {
    case FL_ID_ENTRY_TYPE_C_CALLABLE:
        fl_c_callable_id_entry_free((fl_c_callable_id_entry **)entry);
        break;
    case FL_ID_ENTRY_TYPE_FLOW_CALLABLE:
        fl_flow_callable_id_entry_free((fl_flow_callable_id_entry **)entry);
        break;
    case FL_ID_ENTRY_TYPE_OBJ:
    default:
        assert(FL_FALSE);
        break;
    }
}