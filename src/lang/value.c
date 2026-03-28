#include "value.h"

#include "vecs/fn_variant_vec.h"
#include "lang/scope.h"
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
        assert(sizeof(fl_lang_type_float) == sizeof(double));

        res_data = fl_mem_alloc(sizeof(fl_lang_type_float));
        *((fl_lang_type_float *)res_data) = atof(lit_data);
        break;
    case FL_DATA_TYPE_INT:
        assert(sizeof(fl_lang_type_int) == sizeof(long long));

        res_data = fl_mem_alloc(sizeof(fl_lang_type_int));
        *((fl_lang_type_int *)res_data) = atoll(lit_data);
        break;
    case FL_DATA_TYPE_STRING:
    {
        // substract quotes count data len
        const fl_size len = fl_str_len(lit_data) - 2;
        res_data = fl_mem_alloc_arr(sizeof(fl_char), len);
        // copy string wiouth quotes
        fl_mem_copy((fl_char *)lit_data + 1, res_data, len);
        // add end-of-string char at the end
        ((fl_char *)res_data)[len - 1] = '\0';
        break;
    }
    case FL_DATA_TYPE_ARRAY:
    default:
        assert(FL_FALSE);
        break;
    }

    return fl_value_create(lit_type, res_data);
}