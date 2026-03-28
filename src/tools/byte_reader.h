/**
 * ---- byte_reader.h ----
 *
 * Declares byte reader logic.
 *
 */

#pragma once

// ---- DEPENDENCIES ----

#include "shared/common.h"
#include "vecs/byte_vec.h"

// ---- TYPE DEFINITIONS ----

struct _fl_byte_reader
{
    const fl_byte_vec bytes;
    fl_size iterator;
};
typedef struct _fl_byte_reader fl_byte_reader;

// ---- MACROS ----

#define fl_byte_reader_get(reader, value_type) \
    (*(value_type *)_fl_byte_reader_get(&reader, sizeof(value_type)))

// ---- DECLARATIONS ----

FL_HEADER_FN fl_byte_reader fl_byte_reader_create(const fl_byte_vec bytes);
FL_HEADER_FN fl_unknown *_fl_byte_reader_get(fl_byte_reader *reader, fl_size size);

// ---- HEADER-ONLY DEFINITIONS ----

FL_HEADER_FN fl_byte_reader fl_byte_reader_create(const fl_byte_vec bytes)
{
    return (fl_byte_reader){
        .bytes = bytes,
        .iterator = 0,
    };
}

FL_HEADER_FN fl_unknown *_fl_byte_reader_get(fl_byte_reader *reader, fl_size size)
{
    assert(reader->iterator < reader->bytes.size);
    fl_unknown *value = &reader->bytes.data[reader->iterator];
    reader->iterator += size;
    return value;
}