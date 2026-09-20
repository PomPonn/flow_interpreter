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
    FL_DATA_TRAIT_NONE = 0b00000000,
    FL_DATA_TRAIT_CONST = 0b00000001,
    FL_DATA_TRAIT_VIEW = 0b00000010,
    FL_DATA_TRAIT_REF = 0b00000100,
};
typedef fl_byte fl_data_traits;

struct _fl_data_desc
{
    fl_data_type type;
    fl_data_traits traits;
};
typedef struct _fl_data_desc fl_data_desc;

// ---------- DECLARATIONS ----------

/**
 * @brief Creates an empty data descriptor.
 * @return A descriptor with a null type and no traits.
 */
FL_SEALED_FN fl_data_desc fl_data_desc_empty();

/**
 * @brief Creates a data descriptor for a type without traits.
 * @param type The data type to describe.
 * @return A descriptor for the specified type.
 */
FL_SEALED_FN fl_data_desc fl_data_desc_create(fl_data_type type);

/**
 * @brief Creates a data descriptor for a type with the specified traits.
 * @param type The data type to describe.
 * @param traits The traits associated with the data type.
 * @return A descriptor for the specified type and traits.
 */
FL_SEALED_FN fl_data_desc fl_data_desc_create_traited(fl_data_type type, fl_data_traits traits);

/**
 * @brief Compares two data descriptors for equality.
 * @param d1 The first descriptor.
 * @param d2 The second descriptor.
 * @return FL_TRUE if the descriptors are equal; otherwise, FL_FALSE.
 */
FL_SEALED_FN fl_bool fl_data_desc_equal(fl_data_desc d1, fl_data_desc d2);

/**
 * @brief Checks whether one descriptor matches another descriptor.
 * @param d1 The descriptor whose requirements are checked.
 * @param d2 The descriptor being matched against.
 * @return FL_TRUE if d2 satisfies d1; otherwise, FL_FALSE.
 */
FL_SEALED_FN fl_bool fl_data_desc_match_other(fl_data_desc d1, fl_data_desc d2);

// ---------- DEFINITIONS -----------

FL_SEALED_FN fl_data_desc fl_data_desc_empty()
{
    return (fl_data_desc){
        .type = FL_DATA_TYPE_NULL,
        .traits = FL_DATA_TRAIT_NONE,
    };
}

FL_SEALED_FN fl_data_desc fl_data_desc_create(fl_data_type type)
{
    return (fl_data_desc){
        .type = type,
        .traits = FL_DATA_TRAIT_NONE,
    };
}

FL_SEALED_FN fl_data_desc fl_data_desc_create_traited(fl_data_type type, fl_data_traits traits)
{
    return (fl_data_desc){
        .type = type,
        .traits = traits,
    };
}

FL_SEALED_FN fl_bool fl_data_desc_equal(fl_data_desc d1, fl_data_desc d2)
{
    return fl_bits_is_set(d1.type, d2.type) &&
           d1.traits == d2.traits;
}

FL_SEALED_FN fl_bool fl_data_desc_match_other(fl_data_desc d1, fl_data_desc d2)
{
    // TODO: change this

    if (!fl_bits_is_any(d1.type, d2.type))
        return FL_FALSE;

    if (fl_bits_is_set(d1.traits, FL_DATA_TRAIT_CONST) &&
        !fl_bits_is_set(d2.traits, FL_DATA_TRAIT_CONST))
        return FL_FALSE;

    if (fl_bits_is_set(d1.traits, FL_DATA_TRAIT_REF) &&
        !fl_bits_is_set(d2.traits, FL_DATA_TRAIT_REF))
        return FL_FALSE;

    return FL_TRUE;
}