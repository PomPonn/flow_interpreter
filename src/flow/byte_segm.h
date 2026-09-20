/**
 * ---------- byte_segm.h --------------------
 *
 * Declares readonly bytes segment logic.
 *
 * -------------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "vecs/byte_vec.h"

// ---------- TYPE DEFINITIONS ----------

struct _fl_byte_segm
{
    // Length of the segment.
    fl_size len;

    // Readonly segment data.
    const fl_byte *data;
};

/**
 * @brief Readonly byte segment.
 */
typedef struct _fl_byte_segm fl_byte_segm;

// ---------- DECLARATIONS --------------

/**
 * @brief Creates a readonly byte segment from a byte buffer.
 * @param bytes Pointer to the byte data.
 * @param len Number of bytes in the segment.
 * @return A byte segment referencing the supplied buffer.
 */
FL_INLINE_FN fl_byte_segm fl_byte_segm_create(const fl_byte *bytes, fl_size len);

/**
 * @brief Creates a readonly byte segment from a byte vector.
 * @param vec Pointer to the byte vector.
 * @return A byte segment referencing the vector's data.
 */
FL_INLINE_FN fl_byte_segm fl_byte_segm_create_from_vec(const fl_byte_vec *vec);

// ---------- DEFINITIONS ---------------

FL_INLINE_FN fl_byte_segm fl_byte_segm_create(const fl_byte *bytes, fl_size len)
{
    return (fl_byte_segm){
        .data = bytes,
        .len = len,
    };
}

FL_INLINE_FN fl_byte_segm fl_byte_segm_create_from_vec(const fl_byte_vec *vec)
{
    return (fl_byte_segm){
        .data = vec->data,
        .len = vec->size,
    };
}