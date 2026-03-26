/**
 * ---- byte_vec.test.c ----
 *
 * Defines tests for 'src/revise_me/byte_vec.h'.
 *
 */

#include "vecs/byte_vec.h"
#include <unity/unity.h>

// ---- GLOBALS ----

#define INIT_SIZE 24
fl_byte_vec vec;

// ---- SETUP FUNCTIONS ----

void setUp()
{
    fl_byte_vec_init(&vec, INIT_SIZE);
    for (fl_size i = 0; i < INIT_SIZE; i++)
    {
        vec.data[i] = i;
    }
}

void tearDown()
{
    fl_byte_vec_free(&vec);
}

// ---- TESTS ----

void test_some_function_Sth(void)
{
    // ...
}

// ---- ENTRY ----

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_some_function_Sth);

    return UNITY_END();
}