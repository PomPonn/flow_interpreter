#pragma once

#include "macros/basic.h"
#include "shared/common.h"
#include "shared/error.h"
#include "struct/param_vec.h"
#include "struct/id/c_callable_id_entry.h"

// ---- PRIVATE ----

#define _FL_ID_CHAIN_BUFFER_SIZE 256

// FIX ME
#define _FL_TEMP_LOCATION (fl_file_location_info){.description = ""}

#define _FL_TDESCS(...) \
    (fl_data_type_desc[]) { __VA_ARGS__ }

// requires use of FL_REGISTER_START macro before
#define FL_PARAMS(...)                                              \
    fl_param_vec_init(&params, _FL_SSIZE(_FL_TDESCS(__VA_ARGS__))); \
    for (fl_size _i = 0; _i < params.size; _i++)                    \
    params.data[_i] = _FL_TDESCS(__VA_ARGS__)[_i]

#define _FL_REG_CLL_BASE(parent, constant, name, return_type, ...) \
    FL_PARAMS(__VA_ARGS__);                                        \
    fl_c_callable_id_entry *_FL_MERGE2(name, _entry);              \
    fl_c_callable_id_entry_init(                                   \
        &_FL_MERGE2(name, _entry),                                 \
        _FL_STRINGIFY(name),                                       \
        constant,                                                  \
        return_type,                                               \
        params,                                                    \
        _FL_MERGE4(_fl_, parent, _, name));                        \
    fl_context_register_id(FL_PARAM_CNT, &parent->scope, (fl_id_entry *)_FL_MERGE2(name, _entry))

// ---- PUBLIC ----

#define FL_PARAM_CNT cnt
#define FL_PARAM_ID_CHAIN id_chain

#define fl_throw_import_error(message)                                              \
    {                                                                               \
        fl_char buffer[_FL_ID_CHAIN_BUFFER_SIZE];                                   \
        fl_id_chain_to_string(FL_PARAM_ID_CHAIN, buffer, _FL_ID_CHAIN_BUFFER_SIZE); \
        fl_err_throw(FL_ERR_IMPORT, message, buffer, _FL_TEMP_LOCATION);            \
    }

#define FL_MODULE_CALLABLE(name) \
    fl_unknown *_FL_MERGE4(_fl_, FL_MODULE_NAME, _, name)(fl_arg_vec args)

#define FL_REGISTER_START \
    fl_param_vec params;

#define FL_REGISTER_OBJ(parent, name)                                  \
    fl_scope_id_entry *name;                                           \
    fl_scope_id_entry_init(&name, _FL_STRINGIFY(name), parent);        \
    fl_context_register_id(FL_PARAM_CNT, parent, (fl_id_entry *)name); \
    if (!name)                                                         \
    fl_throw_import_error("Failed to register object")

#define FL_REGISTER_RT_CALLABLE(name, return_type, ...) \
    _FL_REG_CLL_BASE(FL_MODULE_NAME, FL_FALSE, name, return_type, __VA_ARGS__)

#define FL_REGISTER_CT_CALLABLE(name, return_type, ...) \
    _FL_REG_CLL_BASE(FL_MODULE_NAME, FL_TRUE, name, return_type, __VA_ARGS__)
