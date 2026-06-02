/**
 * ---------- file.c --------------------
 *
 * Implements file management logic.
 *
 * --------------------------------------
 */

#include "file.h"

// ---------- DEPENDENCIES --------------

#include "vecs/byte_vec.h"

#include <stdio.h>

// ---------- DEFINITIONS -----------

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

fl_bool fl_file_write(const fl_char *filename, const fl_byte_vec bytes)
{
    FILE *file = fopen(filename, "wb");

    if (!file)
        return FL_FALSE;

    size_t written = fwrite(bytes.data, sizeof(bytes.data[0]), bytes.size, file);
    if (written != bytes.size)
        return FL_FALSE;

    fclose(file);
    return FL_TRUE;
}

fl_bool fl_file_append_str(const fl_char *filename, const fl_char *content)
{
    FILE *file = fopen(filename, "ab");

    if (!file)
        return FL_FALSE;

    if (fputs(content, file) < 0)
        // error occured while appending
        return FL_FALSE;

    fclose(file);
    return FL_TRUE;
}
