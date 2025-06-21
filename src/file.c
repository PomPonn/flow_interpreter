#include "file.h"

#include <stdio.h>
#include <stdlib.h>

char *fl_read_file_as_str(const char *filename) {
  FILE *file = fopen(filename, "rt");
  if (file == NULL) return NULL;

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  uint8_t *buffer = calloc(size + 1, sizeof(char));
  if (buffer == NULL) return NULL;

  fread(buffer, size, 1, file);
  buffer[size] = '\0';  // Null-terminate the string

  fclose(file);
  return buffer;
}

void fl_free_file(uint8_t *buffer) {
  if (buffer != NULL) {
    free(buffer);
  }
}