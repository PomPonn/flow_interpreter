/**
 * ---------- file.h --------------------
 *
 * Declares file managament logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"

// ---------- TYPE DEFINITIONS ----------

FL_FORWARD_STRUCT(fl_byte_vec);

// ---------- DECLARATIONS --------------

fl_char *fl_file_read(const fl_char *filename);

fl_bool fl_file_append_str(const fl_char *filename, const fl_char *content);

fl_bool fl_file_write(const fl_char *filename, const fl_byte_vec bytes);

FL_HEADER_FN fl_void fl_file_content_free(fl_char **buffer)
{
    fl_mem_free(*buffer);
    buffer = FL_NULL;
}
