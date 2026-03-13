#include "meta.h"

// ---- INCLUDES ----
#include "../_framework.h"
#include "struct/call/call_param_vec.h"
#include "struct/call/callable_entry.h"

// ---- DECLARATIONS ----

fl_unknown *_fl_meta_import(fl_call_param_vec *args);

// ---- DEFINITIONS ----

fl_id_node *fl_module_register(fl_context *_FL_PARAM_CNT, fl_id_chain *_FL_PARAM_ID_CHAIN)
{
    _FL_REGISTER_INIT;

    _FL_REGISTER_ROOT(meta);

    if (_FL_PARAM_ID_CHAIN->size == 1)
    {
        // import top level ids

        // register 'import' function
        _FL_REGISTER_CALLABLE_CT(meta, import, FL_DATA_TYPE_NULL, _FL_DATA_TYPES(FL_DATA_TYPE_ARRAY));
    }

    return fl_id_node_seek(_FL_PARAM_CNT->id_tree, _FL_PARAM_ID_CHAIN);
}

fl_unknown *_fl_meta_import(fl_call_param_vec *args)
{

    return FL_NULL;
}
