#include "str.h"

#include <string.h>

fl_bool str_equal(const fl_char *str1, const fl_char *str2)
{
  if (!str1 || !str2)
    return FL_FALSE; // Return false if either string is FL_NULL

  return strcmp(str1, str2) == 0;
}

fl_bool str_equal_n(const fl_char *str1, const fl_char *str2, const fl_size max_count)
{
  if (!str1 || !str2)
    return FL_FALSE; // Return false if either string is FL_NULL

  return strncmp(str1, str2, max_count) == 0;
}

fl_bool str_in(const fl_char *str, const fl_char **arr, fl_size arr_size)
{
  if (!str || !arr || arr_size == 0)
    return FL_FALSE;

  for (fl_size i = 0; i < arr_size; i++)
  {
    if (str_equal(str, arr[i]))
      return FL_TRUE;
  }

  return FL_FALSE;
}

fl_size str_len(const fl_char *str)
{
  return (fl_size)strlen(str);
}