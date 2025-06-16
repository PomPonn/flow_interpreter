#include "str.h"

#include <string.h>

uint8_t str_equal(const char* str1, const char* str2) {
  if (str1 == NULL || str2 == NULL) {
    return 0;  // Return false if either string is NULL
  }
  return strcmp(str1, str2) == 0;
}

uint8_t str_is_in(const char* str, const char** arr, size_t arr_size) {
  if (str == NULL || arr == NULL || arr_size == 0) return 0;

  for (size_t i = 0; i < arr_size; i++) {
    if (str_equal(str, arr[i])) return 1;
  }

  return 0;
}