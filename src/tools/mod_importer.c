#include "mod_importer.h"

#include "tools/lib.h"
#include "shared/error.h"
#include "tokenizer/token_location.h"

#define _FL_MODULE_REGISTER_FN_NAME "fl_module_register"
#define _FL_ID_CHAIN_STRING_MAX_SIZE 512

typedef fl_id_entry *(*fl_module_register_fn)(fl_context *cnt, fl_id_chain *chain);

fl_id_entry *fl_mimp_import_id(fl_context *cnt, const fl_char *id_str)
{
    // parse id string
    fl_id_chain chain;
    fl_id_chain_init_parse(&chain, id_str);

    // check if library dll is already imported
    fl_lib_handle handle = FL_NULL;
    const fl_char *lib_name = chain.data[0];
    fl_uint ind = fl_cstr_vec_find(&cnt->lib_names, lib_name);
    if (ind == FL_FIND_FAIL)
    {
        // load library
        handle = fl_lib_load(lib_name);
        // TODO: change error params
        if (!handle)
            fl_err_throw(FL_ERR_IMPORT, "Couldn't find the library", chain.data[0], FL_TOKEN_LOCATION_EMPTY);

        // cache library data
        fl_lib_handle_vec_push(&cnt->lib_handles, handle);
        fl_cstr_vec_push(&cnt->lib_names, fl_str_dup(lib_name));
    }
    else
        handle = cnt->lib_handles.data[ind];

    fl_module_register_fn reg_fn = (fl_module_register_fn)fl_lib_get_address(handle, _FL_MODULE_REGISTER_FN_NAME);
    if (!reg_fn)
        fl_err_throw(FL_ERR_IMPORT, "Couldn't find module registration function", chain.data[0], FL_TOKEN_LOCATION_EMPTY);

    fl_id_entry *leaf = reg_fn(cnt, &chain);
    if (!leaf)
    {
        fl_char buffer[_FL_ID_CHAIN_STRING_MAX_SIZE];
        fl_id_chain_to_string(&chain, buffer, _FL_ID_CHAIN_STRING_MAX_SIZE);
        fl_err_throw(FL_ERR_IMPORT, "Couldn't import id chain", buffer, FL_TOKEN_LOCATION_EMPTY);
    }

    fl_id_chain_free(&chain);
    return leaf;
}