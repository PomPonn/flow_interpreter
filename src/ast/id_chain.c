#include "id_chain.h"

fl_char *fl_id_chain_to_string(fl_id_chain *chain, fl_char *buffer, fl_size buffer_size)
{
    fl_size total_n = 0;

    for (fl_size i = 0; i < chain->size; i++)
    {
        fl_size n = fl_str_len(chain->data[i]);
        total_n += n;
        if (total_n > buffer_size)
            return FL_NULL;

        fl_mem_copy(chain->data[i], buffer, n);

        if (i == chain->size - 1)
            buffer[total_n] = '\0';
        else
            buffer[total_n] = '.';

        buffer += n;
    }

    return buffer;
}