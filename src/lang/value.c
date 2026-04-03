#include "value.h"

#include "vecs/fn_variant_vec.h"
#include "lang/scope.h"
#include "shared/str.h"
#include "lang/instructions.h"
#include <stdlib.h>

fl_void fl_value_free(fl_value *value)
{
    switch (value->data_type)
    {
    case FL_DATA_TYPE_FN:
        fl_fn_variant_vec_free(value->data);
        break;
    case FL_DATA_TYPE_SCOPE:
        fl_scope_free(value->data);
        break;
    case FL_DATA_TYPE_REF:
        break;
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
        assert(sizeof(fl_flow_type_float) == sizeof(double));

        res_data = fl_mem_alloc(sizeof(fl_flow_type_float));
        *((fl_flow_type_float *)res_data) = atof(lit_data);
        break;
    case FL_DATA_TYPE_INT:
        assert(sizeof(fl_flow_type_int) == sizeof(long long));

        res_data = fl_mem_alloc(sizeof(fl_flow_type_int));
        *((fl_flow_type_int *)res_data) = atoll(lit_data);
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
        case FL_DATA_TYPE_INT:
            return *(fl_flow_type_int *)v1.data == *(fl_flow_type_int *)v2.data;
        case FL_DATA_TYPE_FLOAT:
            return *(fl_flow_type_float *)v1.data == *(fl_flow_type_float *)v2.data;
        default:
            assert(FL_FALSE);
            return FL_FALSE;
        }
    }
}

fl_void fl_value_to_bytes(fl_value v, fl_byte_vec *bytes)
{
    fl_byte_vec_push(bytes, v.data_type);

    switch (v.data_type)
    {
    case FL_DATA_TYPE_STRING:
    {
        const fl_char *str = v.data;
        // add 1 to len to include end of string ('\0')
        fl_byte_vec_push_mem(bytes, str, fl_str_len(str) + 1);
        break;
    }
    case FL_DATA_TYPE_INT:
        fl_byte_vec_push_mem(bytes, v.data, sizeof(fl_flow_type_int));
        break;
    case FL_DATA_TYPE_FLOAT:
        fl_byte_vec_push_mem(bytes, v.data, sizeof(fl_flow_type_float));
        break;
    case FL_DATA_TYPE_SCOPE:
        const fl_scope *scope = v.data;
        const fl_id_entry_vec ids = scope->ids;

        for (fl_size i = 0; i < ids.size; i++)
        {
            fl_byte_vec_push(bytes, FL_INSTR_NEW_ID);
            fl_value_to_bytes(ids.data[i]->value, bytes);
        }

        break;
    case FL_DATA_TYPE_FN:

        // ENDED HERE

        break;
    default:
        assert(FL_FALSE);
        break;
    }
}