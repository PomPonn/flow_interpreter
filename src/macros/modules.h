#pragma once

#include "shared/str.h"
#include "macros/basic.h"
#include "shared/error.h"
#include "lang/scope.h"
#include "lang/value.h"
#include "vecs/value_desc_vec.h"
#include "vecs/fn_variant_vec.h"
#include "vecs/arg_vec.h"

// ---- PRIVATE ----

#define _FL_ID_CHAIN_BUFFER_SIZE 256

// CHANGE ME
#define _FL_TEMP_LOCATION (fl_token_location){.description = ""}

#define _FL_TDESCS(...) \
    (fl_value_desc[]) { __VA_ARGS__ }

// requires use of FL_REGISTER_START macro before
#define _FL_INIT_PARAMS(...)                                             \
    fl_value_desc_vec params;                                            \
    fl_value_desc_vec_init(&params, _FL_SSIZE(_FL_TDESCS(__VA_ARGS__))); \
    for (fl_size _i = 0; _i < params.size; _i++)                         \
    params.data[_i] = _FL_TDESCS(__VA_ARGS__)[_i]

#define _FL_OVERLOAD_FN(name, constant, return_type, ...)                     \
    {                                                                         \
        _FL_INIT_PARAMS(__VA_ARGS__);                                         \
        fl_fn_variant variant;                                                \
        fl_fn_variant_init(&variant, FL_TRUE, constant, return_type, params); \
        variant.pointer = ;                                                   \
        fl_bool success = fl_value_fn_overload(&name->value, variant);        \
        assert(success);                                                      \
    }

// ---- PUBLIC ----

#define FL_PARAM_CNT cnt
#define FL_PARAM_ID_CHAIN id_chain

#define fl_throw_import_error(message)                                              \
    {                                                                               \
        fl_char buffer[_FL_ID_CHAIN_BUFFER_SIZE];                                   \
        fl_id_chain_to_string(FL_PARAM_ID_CHAIN, buffer, _FL_ID_CHAIN_BUFFER_SIZE); \
        value_desc_vec params;                                                      \
        fl_err_throw(FL_ERR_IMPORT, message, buffer, _FL_TEMP_LOCATION);            \
    }

#define FL_REGISTER_FN(parent_id, name)                                                   \
    fl_fn_variant_vec *_FL_MERGE2(name, _data) = fl_mem_alloc(sizeof(fl_fn_variant_vec)); \
    fl_fn_variant_vec_init(_FL_MERGE2(name, _data), 0);                                   \
                                                                                          \
    fl_id_entry *name;                                                                    \
    fl_id_entry_init(                                                                     \
        &name,                                                                            \
        fl_str_dup(_FL_STRINGIFY(name)),                                                  \
        FL_DATA_TYPE_FN,                                                                  \
        fl_value_create(FL_DATA_TYPE_FN, _FL_MERGE2(name, _data)));                       \
                                                                                          \
    fl_context_register_id(FL_PARAM_CNT, parent_id->value.data, name)

#define FL_OVERLOAD_FN_CONST(name, return_type, ...) \
    _FL_OVERLOAD_FN(name, FL_TRUE, return_type, __VA_ARGS__)

#define FL_OVERLOAD_FN(name, return_type, ...) \
    _FL_OVERLOAD_FN(name, FL_FALSE, return_type, __VA_ARGS__)

#define _FL_REG_CLL_BASE(parent_id, name, constant, return_type, ...) \
    _FL_INIT_PARAMS(__VA_ARGS__);                                     \
    fl_fn_id_data *_FL_MERGE2(name, _data);                           \
    fl_fn_id_data_init(                                               \
        &_FL_MERGE2(name, _data),                                     \
        FL_TRUE,                                                      \
        constant,                                                     \
        return_type,                                                  \
        params);                                                      \
                                                                      \
    fl_id_entry *_FL_MERGE2(name, _entry);                            \
    fl_id_entry_init(                                                 \
        &_FL_MERGE2(name, _entry),                                    \
        fl_str_dup(_FL_STRINGIFY(name)),                              \
        FL_DATA_TYPE_FN,                                              \
        _FL_MERGE2(name, _data));                                     \
                                                                      \
    fl_context_register_id(                                           \
        FL_PARAM_CNT,                                                 \
        &((fl_scope_id_data *)parent->data)->scope,                   \
        (fl_id_entry *)_FL_MERGE2(name, _entry))

#define FL_MODULE_FN(name) \
    fl_unknown *_FL_MERGE4(_fl_, FL_MODULE_NAME, _, name)(fl_arg_vec args)

#define FL_REGISTER_SCOPE(parent_scope, name)                           \
    fl_scope *_FL_MERGE2(name, _data) = fl_mem_alloc(sizeof(fl_scope)); \
    fl_scope_init(_FL_MERGE2(name, _data), parent_scope);               \
    fl_id_entry *name;                                                  \
    fl_id_entry_init(                                                   \
        &name,                                                          \
        fl_str_dup(_FL_STRINGIFY(name)),                                \
        FL_DATA_TYPE_SCOPE,                                             \
        fl_value_create(FL_DATA_TYPE_SCOPE, _FL_MERGE2(name, _data)));  \
    fl_context_register_id(FL_PARAM_CNT, parent_scope, (fl_id_entry *)name);

#define FL_REGISTER_RT_FN(name, return_type, ...) \
    _FL_REG_CLL_BASE(FL_MODULE_NAME, FL_FALSE, name, return_type, __VA_ARGS__)

#define FL_REGISTER_CT_FN(name, return_type, ...) \
    _FL_REG_CLL_BASE(FL_MODULE_NAME, FL_TRUE, name, return_type, __VA_ARGS__)
