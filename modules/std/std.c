#include "std.h"

// ---- INCLUDES ----

#include "macros/modules.h"
#include "shared/str.h"
#include "io/io.h"

// ---- DECLARATIONS ----

#define FL_MODULE_NAME std

// ---- DEFINITIONS -----

fl_id_node *fl_module_register(fl_context *FL_PARAM_CNT, fl_id_chain *FL_PARAM_ID_CHAIN)
{
    if (FL_PARAM_ID_CHAIN->size == 0)
        return FL_NULL;

    FL_REGISTER_START;

    FL_REGISTER_OBJ(FL_NULL, FL_MODULE_NAME);

    if (FL_PARAM_ID_CHAIN->size > 1)
    {
        if (fl_str_equal(FL_PARAM_ID_CHAIN->data[1], "io"))
            fl_std_io_register(FL_PARAM_CNT, FL_PARAM_ID_CHAIN, std);
    }

    return fl_id_node_seek(FL_PARAM_CNT->id_tree, FL_PARAM_ID_CHAIN);
}