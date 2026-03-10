#include "id_chain.h"

#include "shared/str.h"

fl_char *fl_id_chain_to_string(fl_id_chain *chain, fl_char *buffer, fl_size buffer_size)
{
    fl_size total_n = 0;
    fl_char *ptr = buffer;

    for (fl_size i = 0; i < chain->size; i++)
    {
        fl_size n = fl_str_len(chain->data[i]);
        total_n += n;
        if (total_n > buffer_size)
            return FL_NULL;

        fl_mem_copy(chain->data[i], ptr, n);
        ptr += n;

        if (i == chain->size - 1)
            *ptr = '\0';
        else
            *ptr++ = '.';
    }

    return buffer;
}

fl_void fl_id_chain_init_parse(fl_id_chain *chain, const fl_char *str)
{
    fl_id_chain_init(chain, 0);

    const fl_char *ptr = str;
    while (*ptr)
    {
        while (*ptr && *ptr != '.')
            *ptr++;

        fl_size n = (fl_size)(ptr - str);
        fl_char *id = fl_mem_alloc(n);
        fl_mem_copy(str, id, n - 1);
        id[n] = '\0';

        fl_id_chain_push(chain, id);

        if (*ptr)
            ptr++;
    }
}