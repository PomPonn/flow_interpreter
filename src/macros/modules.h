#pragma once

#include "shared/str.h"
#include "macros/basic.h"
#include "shared/error.h"
#include "lang/scope.h"
#include "lang/data_type.h"
#include "vecs/value_desc_vec.h"
#include "vecs/fn_variant_vec.h"

// ---- PRIVATE ----

#define _FL_ID_CHAIN_BUFFER_SIZE 256

// CHANGE ME
#define _FL_TEMP_LOCATION (fl_token_location){.description = ""}

#define _FL_TDESCS(...) \
    (fl_value_desc[]) { __VA_ARGS__ }

// requires use of FL_REGISTER_START macro before
#define _FL_INIT_PARAMS(...)                                              \
    fl_value_desc_vec params;                                             \
    fl_value_desc_vec_initc(&params, _FL_SSIZE(_FL_TDESCS(__VA_ARGS__))); \
    for (fl_size _i = 0; _i < params.size; _i++)                          \
    params.data[_i] = _FL_TDESCS(__VA_ARGS__)[_i]

#define _FL_OVERLOAD_FN(name, overload, constant, return_type, ...)                           \
    {                                                                                         \
        _FL_INIT_PARAMS(__VA_ARGS__);                                                         \
        fl_fn_variant variant;                                                                \
        fl_fn_variant_init(&variant, FL_TRUE, constant, return_type, params);                 \
        variant.pointer = _FL_MERGE3(_FL_MERGE4(_fl_, FL_MODULE_NAME, _, name), _, overload); \
        fl_bool success = fl_fn_variant_vec_overload(name->value.data, variant);              \
        assert(success);                                                                      \
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
    fl_fn_variant_vec_init(_FL_MERGE2(name, _data));                                      \
                                                                                          \
    fl_id_entry *name;                                                                    \
    fl_id_entry_init(                                                                     \
        &name,                                                                            \
        fl_str_dup(_FL_STRINGIFY(name)),                                                  \
        FL_DATA_TYPE_FN,                                                                  \
        fl_value_create(FL_DATA_TYPE_FN, _FL_MERGE2(name, _data)));                       \
                                                                                          \
    fl_context_register_id(FL_PARAM_CNT, parent_id->value.data, name)

#define FL_OVERLOAD_FN_CONST(name, overload, return_type, ...) \
    _FL_OVERLOAD_FN(name, overload, FL_TRUE, return_type, __VA_ARGS__)

#define FL_OVERLOAD_FN_ONCE_CONST(name, return_type, ...) \
    FL_OVERLOAD_FN_CONST(name, , return_type, __VA_ARGS__)

#define FL_OVERLOAD_FN(name, overload, return_type, ...) \
    _FL_OVERLOAD_FN(name, overload, FL_FALSE, return_type, __VA_ARGS__)

#define FL_OVERLOAD_FN_ONCE(name, return_type, ...) \
    _FL_OVERLOAD_FN(name, , FL_FALSE, return_type, __VA_ARGS__)

#define FL_MODULE_FN_HEAD(name, overload) \
    fl_value _FL_MERGE3(_FL_MERGE4(_fl_, FL_MODULE_NAME, _, name), _, overload)(fl_value_vec args)

#define FL_MODULE_FN_HEAD_ONCE(name) \
    FL_MODULE_FN_HEAD(name, )

#define FL_REGISTER_SCOPE(parent_scope, name)                           \
    fl_scope *_FL_MERGE2(name, _data) = fl_mem_alloc(sizeof(fl_scope)); \
    fl_scope_init(_FL_MERGE2(name, _data), parent_scope);               \
    fl_id_entry *name;                                                  \
    fl_id_entry_init(                                                   \
        &name,                                                          \
        fl_str_dup(_FL_STRINGIFY(name)),                                \
        FL_DATA_TYPE_SCOPE,                                             \
        fl_value_create(FL_DATA_TYPE_SCOPE, _FL_MERGE2(name, _data)));  \
    fl_context_register_id(FL_PARAM_CNT, parent_scope, (fl_id_entry *)name)