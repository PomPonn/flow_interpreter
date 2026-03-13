#pragma once

#include "shared/common.h"
#include "shared/error.h"
#include "shared/_macros.h"

#define _FL_PARAM_CNT cnt
#define _FL_PARAM_ID_CHAIN id_chain
#define _FL_DATA_TYPES(...) \
    (fl_data_type[]) { __VA_ARGS__ }
#define _FL_ID_CHAIN_BUFFER_SIZE 256
#define _FL_TEMP_LOCATION (fl_location_info){.description = ""}

#define fl_throw_import_error(message)                                           \
    fl_id_chain_to_string(_FL_PARAM_ID_CHAIN, buffer, _FL_ID_CHAIN_BUFFER_SIZE); \
    fl_err_throw(FL_ERR_IMPORT, message, buffer, _FL_TEMP_LOCATION)

#define _FL_REGISTER_INIT                     \
    fl_char buffer[_FL_ID_CHAIN_BUFFER_SIZE]; \
    fl_call_param_vec params

#define _FL_PARAMS(types)                              \
    fl_call_param_vec_init(&params, _FL_SSIZE(types)); \
    for (fl_size _i = 0; _i < params.size; _i++)       \
    fl_call_param_init(&params.data[_i], types[_i])

#define _FL_REGISTER_OBJ(parent, name)                                                                        \
    _FL_MERGE(fl_obj, _entry) * _FL_MERGE(name, _entry);                                                      \
    _FL_MERGE(fl_obj, _entry_init)(&_FL_MERGE(name, _entry), #name);                                          \
    fl_id_node *name = fl_context_register_id(_FL_PARAM_CNT, parent, (fl_id_entry *)_FL_MERGE(name, _entry)); \
    if (!_FL_MERGE(name, _entry))                                                                             \
    fl_throw_import_error("Failed to register object")

#define _FL_REGISTER_ROOT(name) \
    _FL_REGISTER_OBJ(FL_NULL, name)

#define __FL_REG_CLL_BASE(parent, comp, name, return_type, param_types) \
    _FL_PARAMS(param_types);                                            \
    fl_callable_entry *_FL_MERGE(name, _entry);                         \
    fl_callable_entry_init(                                             \
        &_FL_MERGE(name, _entry),                                       \
        #name,                                                          \
        comp,                                                           \
        _FL_MERGE3(_fl_, parent, _FL_MERGE(_, name)),                   \
        return_type,                                                    \
        params);                                                        \
    fl_context_register_id(_FL_PARAM_CNT, parent, (fl_id_entry *)_FL_MERGE(name, _entry))

#define _FL_REGISTER_CALLABLE_RT(parent, name, return_type, param_types) \
    __FL_REG_CLL_BASE(parent, FL_FALSE, name, return_type, param_types)

#define _FL_REGISTER_CALLABLE_CT(parent, name, return_type, param_types) \
    __FL_REG_CLL_BASE(parent, FL_TRUE, name, return_type, param_types)
