#include "meta.h"

// ---- INCLUDES ----
#include "struct/call/call_param_vec.h"
#include "struct/fn/fn_entry.h"

// ---- DECLARATIONS ----

fl_unknown *_fl_meta_import(fl_call_param_vec *args);

// ---- DEFINITIONS ----

fl_bool fl_module_register(fl_context *cnt, fl_id_chain *chain)
{
    fl_obj_entry *meta_object;
    fl_obj_entry_init(&meta_object, "meta");

    fl_id_node *meta = fl_id_node_push_sibling(
        &cnt->id_tree,
        (fl_id_entry *)meta_object);

    if (!meta)
        // TODO: report an error here and in other places
        // object with this name already exists
        return FL_FALSE;

    // register 'import' compile time function (_fl_meta_import)

    fl_call_param_vec params;
    fl_call_param_vec_init(&params, 1);
    fl_call_param_init(&params.data[0], FL_DATA_TYPE_ARRAY);

    fl_fn_entry *import_fn;
    fl_fn_entry_init(
        &import_fn,
        "import",
        FL_TRUE,
        _fl_meta_import,
        FL_DATA_TYPE_NULL,
        params);

    fl_context_register_id(cnt, meta, (fl_id_entry *)import_fn);

    return FL_TRUE;
}

fl_unknown *_fl_meta_import(fl_call_param_vec *args)
{

    return FL_NULL;
}
