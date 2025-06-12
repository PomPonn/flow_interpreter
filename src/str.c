#include "str.h"

#include <string.h>

uint8_t str_equal(const char* str1, const char* str2) {
  if (str1 == NULL || str2 == NULL) {
    return 0;  // Return false if either string is NULL
  }
  return strcmp(str1, str2) == 0;
}