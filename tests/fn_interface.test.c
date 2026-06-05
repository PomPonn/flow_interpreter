/**
 * ---------- fn_interface.test.c -------------------
 *
 * Defines tests for 'flow/fn/fn_interface'.
 *
 * --------------------------------------------------
 */

// ---------- DEPENDENCIES --------------

#include "shared/common.h"
#include "flow/fn/fn_interface.h"
#include "vecs/param_vec.h"

#include <unity/unity.h>

// ---------- GLOBALS ----------

fl_fn_interface intf;

// ----------  SETUP ----------

void setUp()
{
    fl_param_vec params;
    fl_param_vec_init_with(
        &params,
        3,
        (fl_param[]){
            fl_param_create(FL_DATA_TYPE_BOOL),
            fl_param_create_traited(FL_DATA_TYPE_FN, FL_PARAM_TRAITS_REF),
        });

    fl_fn_interface_init(&intf, FL_DATA_TYPE_FLOAT, params);
}

void tearDown()
{
    fl_fn_interface_free(&intf);
}

// ---------- TESTS ----------

void test_fn_interface_overlaps(void)
{
    fl_fn_interface other;

    fl_param_vec params;
    fl_param_vec_init_with(
        &params,
        3,
        (fl_param[]){
            fl_param_create_traited(FL_DATA_TYPE_BOOL, FL_PARAM_TRAITS_CONST),
            fl_param_create_traited(FL_DATA_TYPE_FN, FL_PARAM_TRAITS_REF),
        });

    fl_fn_interface_init(&other, FL_DATA_TYPE_FLOAT, params);

    TEST_ASSERT_TRUE(fl_fn_interface_overlaps(intf, other));

    intf.params.data[1].traits = FL_PARAM_TRAITS_NONE;

    TEST_ASSERT_FALSE(fl_fn_interface_overlaps(intf, other));
}

// ---------- ENTRY ----------

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_fn_interface_overlaps);

    return UNITY_END();
}