/**
 * ---------- byte_vec.test.c ----------
 *
 * Defines tests for 'src/revise_me/byte_vec.h'.
 *
 */

#include "vecs/byte_vec.h"
#include <unity/unity.h>

// ---------- GLOBALS ----------

#define INIT_SIZE 24
fl_byte_vec vec;

// ---------- SETUP FUNCTIONS ----------

void setUp()
{
    fl_byte_vec_init_c(&vec, INIT_SIZE);
    for (fl_size i = 0; i < INIT_SIZE; i++)
        fl_byte_vec_push(&vec, i);
}

void tearDown()
{
    fl_byte_vec_free(&vec);
}

// ---------- TESTS ----------

void test_byte_vec_push_mem_Correctness(void)
{
    fl_long some_val = 155235;
    fl_byte_vec_push_mem(&vec, &some_val, sizeof(some_val));

    fl_long *actual = (fl_long *)(vec.data + vec.size - sizeof(some_val));

    TEST_ASSERT_EQUAL_INT64(some_val, *actual);
    TEST_ASSERT_EQUAL_UINT32(INIT_SIZE + sizeof(some_val), vec.size);
}

// ---------- ENTRY ----------

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_byte_vec_push_mem_Correctness);

    return UNITY_END();
}