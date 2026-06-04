/**
 * ---------- byte_reader.h -------------
 *
 * Declares byte reader logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "shared/str.h"
#include "vecs/byte_vec.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_byte_reader
{
    const fl_byte_vec bytes;
    fl_size iterator;
};
typedef struct _fl_byte_reader fl_byte_reader;

// ---------- MACROS ----------

#define fl_byte_reader_next(reader_ptr, value_type) \
    (*(value_type *)fl_byte_reader_next_bytes(reader_ptr, sizeof(value_type)))

// ---------- DECLARATIONS ----------

FL_HEADER_FN fl_byte_reader fl_byte_reader_create(const fl_byte_vec bytes);
FL_HEADER_FN fl_unknown *_fl_byte_reader_next(fl_byte_reader *reader, fl_size size);
FL_HEADER_FN fl_bool fl_byte_reader_end(fl_byte_reader *reader);
FL_HEADER_FN const fl_char *fl_byte_reader_next_str(fl_byte_reader *reader);

// ---------- HEADER-ONLY DEFINITIONS ----------

FL_HEADER_FN fl_byte_reader fl_byte_reader_create(const fl_byte_vec bytes)
{
    return (fl_byte_reader){
        .bytes = bytes,
        .iterator = 0,
    };
}

FL_HEADER_FN fl_unknown *fl_byte_reader_next_bytes(fl_byte_reader *reader, fl_size size)
{
    assert(reader->iterator < reader->bytes.size);
    fl_unknown *value = &reader->bytes.data[reader->iterator];
    reader->iterator += size;
    return value;
}

FL_HEADER_FN const fl_char *fl_byte_reader_next_str(fl_byte_reader *reader)
{
    const fl_char *start = (const fl_char *)(reader->bytes.data + reader->iterator);
    reader->iterator += fl_str_len(start) + 1;
    return start;
}

FL_HEADER_FN fl_bool fl_byte_reader_end(fl_byte_reader *reader)
{
    return reader->iterator >= reader->bytes.size;
}
