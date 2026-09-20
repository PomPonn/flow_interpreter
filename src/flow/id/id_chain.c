/**
 * ---------- id_chain.c ---------------
 *
 * Implements id chain logic.
 *
 * -------------------------------------
 */

#include "id_chain.h"

// ---------- DEPENDENCIES -------------

#include "shared/str.h"

// ---------- DEFINITIONS --------------

fl_char *fl_id_chain_to_string(const fl_id_chain *chain, fl_char *buffer, fl_size buffer_size)
{
    fl_size total_n = 0;
    fl_char *ptr = buffer;

    for (fl_size i = 0; i < chain->size; i++)
    {
        fl_size n = fl_str_len(chain->data[i]);
        total_n += n;

        if (total_n >= buffer_size)
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

fl_void fl_id_chain_from_str(fl_id_chain *chain, const fl_char *str)
{
    fl_id_chain_init(chain);

    const fl_char *ptr = str;
    while (*ptr)
    {
        while (*ptr && *ptr != '.')
            ptr++;

        fl_size n = (fl_size)(ptr - str);
        fl_char *id = fl_mem_alloc(n + 1);

        fl_mem_copy(str, id, n);
        id[n] = '\0';

        fl_id_chain_push(chain, id);

        if (*ptr)
            ptr++;

        str = ptr;
    }
}