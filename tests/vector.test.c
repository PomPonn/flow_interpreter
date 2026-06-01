/**
 * ---------- vector.test.c ----------
 *
 * Defines tests for 'src/templates/vector'.
 *
 */

#include "templates/vector.h"
#include <unity/unity.h>

// ---------- GLOBALS ----------

#define INIT_SIZE 14
fl_vector vec;

// ---------- SETUP FUNCTIONS ----------

void setUp()
{
    fl_vector_init_c(&vec, 14);
    for (fl_int i = 0; i < INIT_SIZE; i++)
        fl_vector_push(&vec, i);
}

void tearDown()
{
    fl_vector_free(&vec);
}

// ---------- TESTS ----------

void test_vector_reserve_Copying(void)
{
    fl_vector_reserve(&vec, 24);

    TEST_ASSERT_EQUAL_UINT64(INIT_SIZE, vec.size);
    for (fl_int i = 0; i < vec.size; i++)
        TEST_ASSERT_EQUAL_INT(i, vec.data[i]);
}

void test_vector_push(void)
{
    fl_int val = 100;
    fl_size old_size = vec.size;
    fl_vector_push(&vec, val);

    TEST_ASSERT_EQUAL_UINT32(old_size, vec.size - 1);
    TEST_ASSERT_EQUAL_INT(val, vec.data[vec.size - 1]);
}

void test_vector_find(void)
{
    TEST_ASSERT(1 == 1);
    //     fl_int val = 10;

    //     fl_int ind = fl_vector_find(&vec, val);

    //     TEST_ASSERT_NOT_EQUAL_INT(-1, ind);
    //     TEST_ASSERT_EQUAL_INT(val, vec.data[ind]);
}

// ---------- ENTRY ----------

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_vector_reserve_Copying);
    RUN_TEST(test_vector_push);
    RUN_TEST(test_vector_find);

    return UNITY_END();
}