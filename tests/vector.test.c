/**
 * ---------- vector.test.c --------------------
 *
 * Defines tests for 'src/templates/vector'.
 *
 * ---------------------------------------------
 */

// ---------- DEPENDENCIES --------------

#include "templates/vector.h"

#include <unity/unity.h>

// ------------- MACROS -----------------

#define INIT_SIZE 14

// ---------- GLOBALS ------------

fl_vector vec;

// ----------  SETUP -------------

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
    fl_int val = 10;

    fl_int ind = fl_vector_find(&vec, val);

    TEST_ASSERT_NOT_EQUAL_INT(-1, ind);
    TEST_ASSERT_EQUAL_INT(val, vec.data[ind]);
}

void test_vector_put_back(void)
{
    fl_int val = 123;
    fl_int last = vec.data[vec.size - 1];

    fl_vector_put_back(&vec, val);

    TEST_ASSERT_EQUAL_INT(val, vec.data[0]);
    TEST_ASSERT_EQUAL_size_t(INIT_SIZE + 1, vec.size);
    TEST_ASSERT_EQUAL_INT(last, vec.data[vec.size - 1]);
}

void test_vector_pop(void)
{
    fl_int last = vec.data[vec.size - 1];
    fl_int popped = fl_vector_pop(&vec);

    TEST_ASSERT_EQUAL_INT(last, popped);
    TEST_ASSERT_EQUAL_size_t(INIT_SIZE - 1, vec.size);
}

// ---------- ENTRY ----------

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_vector_reserve_Copying);
    RUN_TEST(test_vector_push);
    RUN_TEST(test_vector_find);
    RUN_TEST(test_vector_put_back);
    RUN_TEST(test_vector_pop);

    return UNITY_END();
}