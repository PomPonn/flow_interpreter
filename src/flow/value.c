/**
 * ---------- value.c ----------------
 *
 * Implements value logic.
 *
 * -----------------------------------
 */

#include "value.h"

// ---------- DEPENDENCIES ----------

#include "vecs/fn_variant_vec.h"
#include "vecs/byte_vec.h"
#include "flow/scope.h"
#include "shared/str.h"
#include "flow/instr/instruction.h"
#include "tools/byte_reader.h"

#include <stdlib.h>

// ---------- DEFINITIONS -----------

fl_void fl_value_free(fl_value *value)
{
    switch (value->data_type)
    {
    case FL_DATA_TYPE_FN:
        fl_fn_variant_vec_free(value->data);
        break;
    case FL_DATA_TYPE_SCOPE:
        // ???
    case FL_DATA_TYPE_REF:
    default:
        fl_mem_free(value->data);
        break;
    }

    value->data = FL_NULL;
    value->data_type = FL_DATA_TYPE_NULL;
}

fl_value fl_value_from_literal(fl_data_type lit_type, fl_unknown *lit_data)
{
    fl_unknown *res_data = FL_NULL;

    // decide how to convert data using literal type
    switch (lit_type)
    {
    case FL_DATA_TYPE_FLOAT:
        assert(sizeof(fl_flow_float) == sizeof(double));

        res_data = fl_mem_alloc_t(fl_flow_float);
        *((fl_flow_float *)res_data) = atof(lit_data);
        break;
    case FL_DATA_TYPE_INT:
        assert(sizeof(fl_flow_int) == sizeof(long long));

        res_data = fl_mem_alloc_t(fl_flow_int);
        *((fl_flow_int *)res_data) = atoll(lit_data);
        break;
    case FL_DATA_TYPE_STRING:
    {
        // substract quotes count data len
        const fl_size len = fl_str_len(lit_data) - 2;
        res_data = fl_mem_alloc_arr(sizeof(fl_char), len + 1);
        // copy string wiouth quotes
        fl_mem_copy((fl_char *)lit_data + 1, res_data, len);
        // add end-of-string char at the end
        ((fl_char *)res_data)[len] = '\0';
        break;
    }
    case FL_DATA_TYPE_ARRAY:
    default:
        assert(FL_FALSE);
        break;
    }

    return fl_value_create(lit_type, res_data);
}

fl_bool fl_value_indentical(fl_value v1, fl_value v2)
{
    if (v1.data_type != v2.data_type)
        return FL_FALSE;

    if (v1.data == v2.data)
        return FL_TRUE;
    else
    {
        switch (v1.data_type)
        {
        case FL_DATA_TYPE_STRING:
            return fl_str_equal(v1.data, v2.data);
        case FL_DATA_TYPE_BOOL:
            return *(fl_flow_bool *)v1.data == *(fl_flow_bool *)v2.data;
        case FL_DATA_TYPE_INT:
            return *(fl_flow_int *)v1.data == *(fl_flow_int *)v2.data;
        case FL_DATA_TYPE_FLOAT:
            return *(fl_flow_float *)v1.data == *(fl_flow_float *)v2.data;
        default:
            assert(FL_FALSE);
            return FL_FALSE;
        }
    }
}

fl_void fl_value_to_bytes(fl_value v, fl_byte_vec *bytes)
{
    fl_byte_vec_push_t(bytes, v.data_type, fl_data_type);
    const fl_size size = fl_value_mem_size(v);

    if (fl_value_is_size_complex(v))
        fl_byte_vec_push_t(bytes, size, fl_size);

    switch (v.data_type)
    {
    case FL_DATA_TYPE_STRING:
    case FL_DATA_TYPE_BOOL:
    case FL_DATA_TYPE_INT:
    case FL_DATA_TYPE_FLOAT:
        fl_byte_vec_push_mem(bytes, v.data, size);
        break;
    case FL_DATA_TYPE_SCOPE:
        fl_scope_to_bytes(v.data, bytes);
        break;
    case FL_DATA_TYPE_FN:
    {
        fl_fn_variant_vec *vars = v.data;
        // assert only one variant for now
        assert(vars->size == 1);
        fl_fn_variant *var = &vars->data[0];

        fl_fn_interface_to_bytes(var->interface, bytes);

        assert(!var->c_defined);
        fl_byte_vec_merge(bytes, var->body);
        break;
    }

    default:
        assert(FL_FALSE);
        break;
    }
}

fl_size fl_value_mem_size(fl_value value)
{
    switch (value.data_type)
    {
    case FL_DATA_TYPE_ARRAY:
        // TODO
        assert(FL_FALSE);
        return 0;
    case FL_DATA_TYPE_SCOPE:
        // TODO
        // make sure it matches bytes count returned by fl_scope_to_bytes()
        assert(FL_FALSE);
        return 0;
    case FL_DATA_TYPE_FN:
    {
        fl_fn_variant_vec *vars = value.data;
        // assert only one variant for now
        assert(vars->size == 1);
        fl_fn_variant *var = &vars->data[0];

        assert(!var->c_defined);
        return fl_fn_interface_mem_size(var->interface) + var->body->size;
    }
        assert(FL_FALSE);
    case FL_DATA_TYPE_BOOL:
        return sizeof(fl_flow_bool);
    case FL_DATA_TYPE_FLOAT:
        return sizeof(fl_flow_float);
    case FL_DATA_TYPE_INT:
        return sizeof(fl_flow_int);
    case FL_DATA_TYPE_STRING:
        // +1 char for '\0'
        return fl_str_len(value.data) + sizeof(fl_char);
    case FL_DATA_TYPE_REF:
        // is it correct?
        return sizeof(fl_id_entry);
    default:
        return 0;
    }
}

fl_value fl_value_copy(fl_value value)
{
    fl_value copied = fl_value_create(value.data_type, FL_NULL);

    switch (value.data_type)
    {
    case FL_DATA_TYPE_SCOPE:
        // TODO
        assert(FL_FALSE);
        break;
    case FL_DATA_TYPE_FN:
        assert(FL_FALSE);
        break;
    case FL_DATA_TYPE_BOOL:
    case FL_DATA_TYPE_INT:
    case FL_DATA_TYPE_FLOAT:
    case FL_DATA_TYPE_STRING:
    {
        const fl_size size = fl_value_mem_size(value);
        copied.data = fl_mem_alloc(size);
        fl_mem_copy(value.data, copied.data, size);
        break;
    }
    default:
        break;
    }

    return copied;
}

fl_value fl_value_from_bytes(fl_byte_reader *reader)
{
    const fl_data_type type = fl_byte_reader_next(reader, fl_data_type);
    fl_value val = fl_value_create(type, FL_NULL);

    switch (type)
    {
    case FL_DATA_TYPE_STRING:
        val.data = fl_str_dup(fl_byte_reader_next_str(reader));
        break;
    case FL_DATA_TYPE_INT:
    case FL_DATA_TYPE_FLOAT:
    case FL_DATA_TYPE_BOOL:
    {
        const fl_size size = fl_value_mem_size(val);
        val.data = fl_byte_reader_next_bytes(reader, size);
        break;
    }
    default:
        assert(FL_FALSE);
        break;
    }

    return val;
}