#include "str.h"

#include <string.h>

// ---- IMPLEMENTATIONS ----

fl_bool fl_chr_is_digit(fl_char chr)
{
    return chr >= '0' && chr <= '9';
}

fl_char *fl_str_find_any(const fl_char *str, const fl_char *chrs)
{
    return strpbrk(str, chrs);
}

fl_bool fl_str_equal(const fl_char *str1, const fl_char *str2)
{
    if (!str1 || !str2)
        return FL_FALSE; // Return false if either string is FL_NULL

    return strcmp(str1, str2) == 0;
}

fl_bool fl_chr_is_any(fl_char chr, fl_char *pool)
{
    while (*pool)
    {
        if (chr == *pool)
            return FL_TRUE;
        pool++;
    }

    return FL_FALSE;
}

fl_bool fl_str_equal_n(const fl_char *str1, const fl_char *str2, const fl_size max_count)
{
    if (!str1 || !str2)
        return FL_FALSE; // Return false if either string is FL_NULL

    return strncmp(str1, str2, max_count) == 0;
}

fl_bool fl_str_in(const fl_char *str, const fl_char **arr, fl_size arr_size)
{
    if (!str || !arr || arr_size == 0)
        return FL_FALSE;

    for (fl_size i = 0; i < arr_size; i++)
    {
        if (fl_str_equal(str, arr[i]))
            return FL_TRUE;
    }

    return FL_FALSE;
}

fl_bool fl_str_has(const fl_char *str, fl_char chr)
{
    return strchr(str, chr) != FL_NULL;
}

fl_char *fl_str_dup(const fl_char *src)
{
    return _strdup(src);
}

fl_size fl_str_len(const fl_char *str)
{
    return (fl_size)strlen(str);
}

fl_bool fl_str_is_num(const fl_char *str, fl_bool *floating)
{
    const fl_size len = fl_str_len(str);
    *floating = FL_FALSE;

    for (fl_size i = 0; i < len; i++)
    {
        if (str[i] == '.')
        {
            if (*floating || i == 0 || i == len - 1)
                return FL_FALSE;
            else
                *floating = FL_TRUE;
        }
        else if (!fl_chr_is_digit(str[i]))
            return FL_FALSE;
    }

    return FL_TRUE;
}

fl_char *fl_str_find(const fl_char *str, fl_char chr)
{
    return strchr(str, chr);
}

fl_long fl_str_index(const fl_char *str, fl_char chr)
{
    for (fl_long i = 0; *str; i++)
    {
        if (*str == chr)
            return i;

        str++;
    }

    return -1;
}