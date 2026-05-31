#include "value_vec.h"

fl_void fl_value_vec_to_bytes(const fl_value_vec *vec, fl_byte_vec *bytes)
{
    for (fl_size i = 0; i < vec->size; i++)
    {
        fl_value curr = vec->data[i];

        if (fl_value_is_size_complex(curr))
        {
            const fl_size size = fl_value_mem_size(curr);
            fl_byte_vec_push_t(bytes, size, fl_size);
        }

        fl_value_to_bytes(curr, bytes);
    }
}