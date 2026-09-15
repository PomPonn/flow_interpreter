/**
 * ---------- macros/modules.h ------------------
 *
 * Declares module macros.
 *
 * ----------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "macros/basic.h"
#include "shared/str.h"
#include "shared/error.h"
#include "flow/scope.h"
#include "flow/data_type.h"
#include "vecs/data_desc_vec.h"
#include "vecs/fn_variant_vec.h"
#include "flow/fn/fn_interface.h"

// ------------- MACROS -----------------

// ====== PRIVATE ======

// CHANGE ME
#define _FL_TEMP_LOCATION (fl_token_location){.description = ""}

#define _FL_TDESCS(...) \
    (fl_data_desc[]) { __VA_ARGS__ }

#define _fl_data_descS(...)                                               \
    fl_data_desc_vec params;                                              \
    fl_data_desc_vec_init_c(&params, _FL_SSIZE(_FL_TDESCS(__VA_ARGS__))); \
    for (fl_size _i = 0; _i < params._capacity; _i++)                     \
    fl_data_desc_vec_push(&params, _FL_TDESCS(__VA_ARGS__)[_i])

#define _FL_OVERLOAD_FN(name, overload, return_type, ...)                                                                 \
    {                                                                                                                     \
        _fl_data_descS(__VA_ARGS__);                                                                                      \
        fl_fn_interface interface;                                                                                        \
        fl_fn_interface_init(&interface, return_type, params);                                                            \
        fl_fn_variant var;                                                                                                \
        fl_fn_variant_init(&var, FL_TRUE, interface, _FL_MERGE3(_FL_MERGE4(_fl_, FL_MODULE_NAME, _, name), _, overload)); \
        fl_bool success = fl_fn_variant_vec_overload(name->value.data, var);                                              \
        assert(success);                                                                                                  \
    }

// ====== PUBLIC ======

#define fl_fl_throw_import_error(message)                                \
    {                                                                    \
        fl_char buffer[FL_ID_CHAIN_MAX_SIZE];                            \
        fl_id_chain_to_string(id_chain, buffer, FL_ID_CHAIN_MAX_SIZE);   \
        param_vec params;                                                \
        fl_err_throw(FL_ERR_IMPORT, message, buffer, _FL_TEMP_LOCATION); \
    }

#define FL_REGISTER_FN(name, scope)                                                 \
    fl_fn_variant_vec *_FL_MERGE2(name, _data) = fl_mem_alloc_t(fl_fn_variant_vec); \
    fl_fn_variant_vec_init(_FL_MERGE2(name, _data));                                \
    fl_id_entry *name;                                                              \
    fl_id_entry_init(                                                               \
        &name,                                                                      \
        fl_str_dup(_FL_STRINGIFY(name)),                                            \
        FL_DATA_TYPE_FN,                                                            \
        fl_value_create(FL_DATA_TYPE_FN, _FL_MERGE2(name, _data)));                 \
    fl_id_entry_vec_push(&scope->ids, name);

#define FL_OVERLOAD_FN(name, overload, return_type, ...) \
    _FL_OVERLOAD_FN(name, overload, return_type, __VA_ARGS__)

#define FL_OVERLOAD_FN_ONCE(name, return_type, ...) \
    _FL_OVERLOAD_FN(name, , return_type, __VA_ARGS__)

#define FL_MODULE_FN_HEAD(name, overload) \
    fl_value _FL_MERGE3(_FL_MERGE4(_fl_, FL_MODULE_NAME, _, name), _, overload)(fl_value_vec args)

#define FL_MODULE_FN_HEAD_ONCE(name) \
    FL_MODULE_FN_HEAD(name, )

#define FL_REGISTER_SCOPE(parent_scope, name)                          \
    fl_scope *_FL_MERGE2(name, _scope) = fl_mem_alloc_t(fl_scope);     \
    fl_scope_init(&_FL_MERGE2(name, _scope), parent_scope);            \
    fl_id_entry *name;                                                 \
    fl_id_entry_init(                                                  \
        &name,                                                         \
        fl_str_dup(_FL_STRINGIFY(name)),                               \
        FL_DATA_TYPE_SCOPE,                                            \
        fl_value_create(FL_DATA_TYPE_SCOPE, _FL_MERGE2(name, _scope)), \
        FL_FALSE);