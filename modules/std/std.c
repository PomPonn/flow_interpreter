#include "std.h"

// ---- INCLUDES ----

#include "macros/modules.h"
#include "shared/str.h"
#include "io/io.h"

// ---- DECLARATIONS ----

#define FL_MODULE_NAME std

// ---- DEFINITIONS -----

fl_id_entry *fl_module_register(fl_context *FL_PARAM_CNT, fl_id_chain *FL_PARAM_ID_CHAIN)
{
    if (FL_PARAM_ID_CHAIN->size == 0)
        return FL_NULL;

    FL_REGISTER_START;

    FL_REGISTER_SCOPE(FL_NULL, FL_MODULE_NAME);

    if (FL_PARAM_ID_CHAIN->size > 1)
    {
        if (fl_str_equal(FL_PARAM_ID_CHAIN->data[1], "io"))
            fl_std_io_register(FL_PARAM_CNT, FL_PARAM_ID_CHAIN, &std->scope);
    }

    fl_uint offset;
    fl_int_vec inds = fl_scope_resolve_id(&FL_PARAM_CNT->main_scope, FL_PARAM_ID_CHAIN, &offset);
    fl_id_entry *id = fl_scope_traverse(&FL_PARAM_CNT->main_scope, inds, offset);
    fl_int_vec_free(&inds);
    return id;
}