#include "file.h"

#include <stdio.h>

fl_char *fl_file_read(const fl_char *filename)
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
    buffer[size] = '\0'; // null-terminate the string

    fclose(file);
    return buffer;
}

fl_void fl_file_append(const fl_char *filename, fl_char *data)
{
    FILE *file = fopen(filename, "a");

    assert(file);

    fputs(data, file);

    fclose(file);
}