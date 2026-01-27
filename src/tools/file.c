#include "file.h"

#include "shared/memory.h"

#include <stdio.h>

char *fl_file_read(const fl_char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
        return FL_NULL;

    fseek(file, 0, SEEK_END);
    fl_size size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fl_char *buffer = fl_mem_alloc_arr(sizeof(fl_char), size + 1);
    if (!buffer)
        return FL_NULL;

    fread(buffer, size, 1, file);
    buffer[size] = '\0'; // Null-terminate the string

    fclose(file);
    return buffer;
}

void fl_file_append(const fl_char *filename, char *data)
{
    FILE *file = fopen(filename, "a");

    assert(file);

    fprintf(file, data);

    fclose(file);
}

fl_void fl_file_free(fl_char *buffer)
{
    fl_mem_free(buffer);
}