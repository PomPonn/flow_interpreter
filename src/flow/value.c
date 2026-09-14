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
    if (!fl_bitfield_is_set(value->desc.traits, FL_DATA_TRAITS_IMMUTABLE))
    {
        switch (value->desc.type)
        {
        case FL_DATA_TYPE_FN:
            fl_fn_variant_vec_free(value->data);
            break;
            // ???
        case FL_DATA_TYPE_CODE_PTR:
            break;
        case FL_DATA_TYPE_SCOPE:
        default:
            fl_mem_free(value->data);
            break;
        }
    }

    value->data = FL_NULL;
    value->desc = fl_data_desc_empty();
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

    fl_data_desc desc = fl_data_desc_create_traited(lit_type, FL_DATA_TRAITS_CONST);
    return fl_value_create_desc(desc, res_data);
}

fl_bool fl_value_indentical(fl_value v1, fl_value v2)
{
    if (v1.desc.type != v2.desc.type)
        return FL_FALSE;

    if (v1.data == v2.data)
        return FL_TRUE;
    else
    {
        switch (v1.desc.type)
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
    fl_byte_vec_push_t(bytes, v.desc.type, fl_data_type);

    switch (v.desc.type)
    {
    case FL_DATA_TYPE_STRING:
    case FL_DATA_TYPE_BOOL:
    case FL_DATA_TYPE_INT:
    case FL_DATA_TYPE_FLOAT:
        fl_byte_vec_push_mem(bytes, v.data, fl_value_mem_size(v));
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
        assert(!var->c_defined);

        fl_fn_interface_to_bytes(var->interface, bytes);

        assert(*var->body_ptr == FL_INSTR_START_BLOCK);

        const fl_size block_len = *(fl_size *)(var->body_ptr + sizeof(fl_instruction));
        const fl_byte *actual_body = var->body_ptr + sizeof(fl_instruction) + sizeof(fl_size);

        fl_byte_vec_push_t(bytes, block_len, fl_size);
        fl_byte_vec_push_mem(bytes, actual_body, block_len);
        fl_byte_vec_push(bytes, FL_INSTR_BODY_END);
        break;
    }
    case FL_DATA_TYPE_ARRAY:
    {
        fl_value_vec *items = v.data;

        fl_byte_vec_push_t(bytes, items->size, fl_size);

        fl_value_vec_to_bytes(items, bytes);

        break;
    }
    default:
        assert(FL_FALSE);
        break;
    }
}

fl_size fl_value_mem_size(fl_value value)
{
    switch (value.desc.type)
    {
    case FL_DATA_TYPE_BOOL:
        return sizeof(fl_flow_bool);
    case FL_DATA_TYPE_FLOAT:
        return sizeof(fl_flow_float);
    case FL_DATA_TYPE_INT:
        return sizeof(fl_flow_int);
    case FL_DATA_TYPE_STRING:
        // +1 char for '\0'
        return fl_str_len(value.data) + sizeof(fl_char);
    default:
        assert(FL_FALSE);
        return 0;
    }
}

fl_value fl_value_copy(fl_value value)
{
    fl_value copied = fl_value_create(value.desc.type, FL_NULL);

    switch (value.desc.type)
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
    case FL_DATA_TYPE_ARRAY:
    {
        const fl_size size = fl_byte_reader_next(reader, fl_size);
        fl_value_vec *items = fl_mem_alloc_t(fl_value_vec);
        fl_value_vec_init_c(items, size);

        for (fl_size i = 0; i < size; i++)
            fl_value_vec_push(items, fl_value_from_bytes(reader));

        val.data = items;
        break;
    }
    case FL_DATA_TYPE_FN:
    {
        const fl_size params_count = fl_byte_reader_next(reader, fl_size);

        fl_data_desc_vec params;
        fl_cstr_vec labels;
        fl_data_desc_vec_init_c(&params, params_count);
        fl_cstr_vec_init_c(&labels, params_count);

        // parse paramaters
        fl_data_desc desc = fl_data_desc_create(FL_DATA_TYPE_ANY);

        for (fl_size i = 0; i < params_count; i++)
        {
            const fl_char *label = fl_byte_reader_next_str(reader);
            fl_cstr_vec_push(&labels, fl_str_dup(label));
            fl_data_desc_vec_push(&params, desc);
        }

        // handle body
        const fl_byte *body_ptr = fl_byte_reader_current_ptr(reader);
        const fl_size body_len = fl_byte_reader_next(reader, fl_size);
        // +1 for FL_INSTR_BODY_END
        fl_byte_reader_skip_bytes(reader, body_len + 1);

        fl_fn_interface intf;
        fl_fn_interface_init(&intf, FL_DATA_TYPE_ANY, params);
        fl_fn_interface_set_labels(&intf, labels);

        fl_fn_variant variant;
        fl_fn_variant_init(&variant, FL_FALSE, intf, body_ptr);

        fl_fn_variant_vec *vars = fl_mem_alloc_t(fl_fn_variant_vec);
        fl_fn_variant_vec_init_with(vars, 1, (fl_fn_variant[]){variant});

        val.data = vars;
        break;
    }
    default:
        assert(FL_FALSE);
        break;
    }

    return val;
}