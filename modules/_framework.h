#pragma once

#include "shared/common.h"
#include "shared/error.h"
#include "shared/_macros.h"

// ---- PRIVATE ----

#define _FL_ID_CHAIN_BUFFER_SIZE 256
#define _FL_TEMP_LOCATION (fl_location_info){.description = ""}

#define _FL_REG_CLL_BASE(parent, comp, name, return_type, param_types) \
    FL_PARAMS(param_types);                                            \
    fl_callable_id_entry *_FL_MERGE(name, _entry);                     \
    fl_callable_id_entry_init(                                         \
        &_FL_MERGE(name, _entry),                                      \
        #name,                                                         \
        comp,                                                          \
        _FL_MERGE3(_fl_, parent, _FL_MERGE(_, name)),                  \
        return_type,                                                   \
        params);                                                       \
    fl_context_register_id(FL_PARAM_CNT, parent, (fl_id_entry *)_FL_MERGE(name, _entry))

// ---- PUBLIC ----

#define FL_PARAM_CNT cnt
#define FL_PARAM_ID_CHAIN id_chain
#define FL_DATA_TYPES(...) \
    (fl_data_type[]) { __VA_ARGS__ }

#define fl_throw_import_error(message)                                          \
    fl_id_chain_to_string(FL_PARAM_ID_CHAIN, buffer, _FL_ID_CHAIN_BUFFER_SIZE); \
    fl_err_throw(FL_ERR_IMPORT, message, buffer, _FL_TEMP_LOCATION)

#define FL_MODULE_CALLABLE(name) \
    fl_unknown *_FL_MERGE3(_fl_, FL_MODULE, _FL_MERGE(_, name))(fl_call_param_vec * args)

#define FL_REGISTER_START                     \
    fl_char buffer[_FL_ID_CHAIN_BUFFER_SIZE]; \
    fl_call_param_vec params;                 \
    FL_REGISTER_OBJ(FL_NULL, FL_MODULE)

#define FL_PARAMS(types)                               \
    fl_call_param_vec_init(&params, _FL_SSIZE(types)); \
    for (fl_size _i = 0; _i < params.size; _i++)       \
    fl_call_param_init(&params.data[_i], types[_i])

#define FL_REGISTER_OBJ(parent, name)                                                                        \
    fl_obj_id_entry *_FL_MERGE(name, _entry);                                                                \
    fl_obj_id_entry_init(&_FL_MERGE(name, _entry), #name);                                                   \
    fl_id_node *name = fl_context_register_id(FL_PARAM_CNT, parent, (fl_id_entry *)_FL_MERGE(name, _entry)); \
    if (!_FL_MERGE(name, _entry))                                                                            \
    fl_throw_import_error("Failed to register object")

#define FL_REGISTER_ROOT(name) \
    FL_REGISTER_OBJ(FL_NULL, name)

#define FL_REGISTER_CALLABLE_RT(parent, name, return_type, param_types) \
    _FL_REG_CLL_BASE(parent, FL_FALSE, name, return_type, param_types)

#define FL_REGISTER_CALLABLE_CT(parent, name, return_type, param_types) \
    _FL_REG_CLL_BASE(parent, FL_TRUE, name, return_type, param_types)
