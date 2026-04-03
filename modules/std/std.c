#include "std.h"

// ---------- INCLUDES ----------

#include "macros/modules.h"
#include "shared/str.h"
#include "io/io.h"

// ---------- DEFINITIONS -----------

FL_FLOW_MODULE fl_value fl_module_resolve_id_value(const fl_id_chain *id_chain)
{
    if (id_chain->size == 0)
        return FL_VALUE_EMPTY;

    if (id_chain->size == 1)
    {
        // return this module as a scope
        fl_scope *std_scope = fl_mem_alloc(sizeof(fl_scope));
        fl_scope_init(std_scope, FL_NULL);

        // import everything from this module

        // import 'io' module
        fl_std_io_register(std_scope, id_chain);

        return fl_value_create(FL_DATA_TYPE_SCOPE, std_scope);
    }

    // pass id resolving to inner submodules
    if (fl_str_equal(id_chain->data[1], "io"))
        return fl_std_io_register(FL_NULL, id_chain);
    else
        return FL_VALUE_EMPTY;
}