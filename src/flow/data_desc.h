/**
 * ---------- data_desc.h ---------------
 *
 * Declares data descriptor logic.
 *
 * --------------------------------------
 */

#pragma once

// ---------- DEPENDENCIES --------------

#include "flow/data_type.h"

// ---------- TYPE DEFINITIONS ----------

enum _fl_data_traits
{
    // No traits.
    FL_DATA_TRAIT_NONE = 0b00000000,

    // Any trait acceptable.
    FL_DATA_TRAIT_ANY = 0b00000001,

    // Data is immutable.
    FL_DATA_TRAIT_CONST = 0b00000010,

    // Data is a view of something. Shouldn't be freed.
    FL_DATA_TRAIT_VIEW = 0b00000100,

    // Data is a pointer to some id entry.
    FL_DATA_TRAIT_REF = 0b00001000,
};

/**
 * @brief Data traits.
 */
typedef fl_byte fl_data_traits;

struct _fl_data_desc
{
    // Data type.
    fl_data_type type;

    // Data traits.
    fl_data_traits traits;
};

/**
 * @brief Data descriptor.
 */
typedef struct _fl_data_desc fl_data_desc;

// ---------- DECLARATIONS ----------

/**
 * @brief Creates an empty data descriptor.
 * @return A descriptor with a null type and no traits.
 */
FL_INLINE_FN fl_data_desc fl_data_desc_empty();

/**
 * @brief Creates a data descriptor for a type without traits.
 * @param type The data type to describe.
 * @return A descriptor for the specified type.
 */
FL_INLINE_FN fl_data_desc fl_data_desc_create(fl_data_type type);

/**
 * @brief Creates a data descriptor for a type with the specified traits.
 * @param type The data type to describe.
 * @param traits The traits associated with the data type.
 * @return A descriptor for the specified type and traits.
 */
FL_INLINE_FN fl_data_desc fl_data_desc_create_traited(fl_data_type type, fl_data_traits traits);

/**
 * @brief Compares two data descriptors for equality.
 * @param d1 The first descriptor.
 * @param d2 The second descriptor.
 * @return FL_TRUE if the descriptors are equal; otherwise, FL_FALSE.
 */
FL_INLINE_FN fl_bool fl_data_desc_equal(fl_data_desc d1, fl_data_desc d2);

/**
 * @brief Checks whether one descriptor matches weakly another descriptor.
 * @param d1 The descriptor whose requirements are checked.
 * @param d2 The descriptor being matched against.
 * @return FL_TRUE if d2 satisfies d1; otherwise, FL_FALSE.
 */
FL_INLINE_FN fl_bool fl_data_desc_is_weak_match(fl_data_desc d1, fl_data_desc d2);

/**
 * @brief Checks whether one descriptor matches stronglu another descriptor.
 * @param d1 The descriptor whose requirements are checked.
 * @param d2 The descriptor being matched against.
 * @return FL_TRUE if d2 satisfies d1; otherwise, FL_FALSE.
 */
FL_INLINE_FN fl_bool fl_data_desc_is_strong_match(fl_data_desc d1, fl_data_desc d2);

// ---------- DEFINITIONS -----------

FL_INLINE_FN fl_data_desc fl_data_desc_empty()
{
    return (fl_data_desc){
        .type = FL_DATA_TYPE_NULL,
        .traits = FL_DATA_TRAIT_NONE,
    };
}

FL_INLINE_FN fl_data_desc fl_data_desc_create(fl_data_type type)
{
    return (fl_data_desc){
        .type = type,
        .traits = FL_DATA_TRAIT_NONE,
    };
}

FL_INLINE_FN fl_data_desc fl_data_desc_create_traited(fl_data_type type, fl_data_traits traits)
{
    return (fl_data_desc){
        .type = type,
        .traits = traits,
    };
}

FL_INLINE_FN fl_bool fl_data_desc_equal(fl_data_desc d1, fl_data_desc d2)
{
    return fl_bit_is_set(d1.type, d2.type) &&
           d1.traits == d2.traits;
}

FL_INLINE_FN fl_bool fl_data_desc_is_weak_match(fl_data_desc d1, fl_data_desc d2)
{
    // TODO: change this

    // It's easy to convert to segment - almost every type can be it (maybe change it later)
    if (!fl_bit_is_any(d1.type, d2.type) && d1.type != FL_DATA_TYPE_CODE_SEGM)
        return FL_FALSE;

    if (fl_bit_is_set(d1.traits, FL_DATA_TRAIT_ANY))
        return FL_TRUE;

    if (fl_bit_is_set(d1.traits, FL_DATA_TRAIT_CONST) &&
        !fl_bit_is_set(d2.traits, FL_DATA_TRAIT_CONST))
        return FL_FALSE;

    if (fl_bit_is_set(d1.traits, FL_DATA_TRAIT_REF) &&
        !fl_bit_is_set(d2.traits, FL_DATA_TRAIT_REF))
        return FL_FALSE;

    return FL_TRUE;
}

FL_INLINE_FN fl_bool fl_data_desc_is_strong_match(fl_data_desc d1, fl_data_desc d2)
{
    if (!fl_bit_is_any(d1.type, d2.type))
        return FL_FALSE;

    if (fl_bit_is_set(d1.traits, FL_DATA_TRAIT_ANY))
        return FL_TRUE;

    if (fl_bit_is_set(d1.traits, FL_DATA_TRAIT_REF) !=
        fl_bit_is_set(d2.traits, FL_DATA_TRAIT_REF))
        return FL_FALSE;

    return FL_TRUE;
}