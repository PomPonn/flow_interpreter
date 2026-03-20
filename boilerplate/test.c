/**
 * ---- <module filename>.test.c ----
 *
 * Defines tests for '<path to the module>'.
 *
 */

// #include "<module include>"
#include <unity/unity.h>

// ---- GLOBALS ----

// ...

// ---- SETUP FUNCTIONS ----

void setUp()
{
    // Setup code...
}

void tearDown()
{
    // Cleanup code...
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