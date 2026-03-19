/**
 * ---- optimalizer.test.c ----
 *
 * Defines tests for 'src/optimalizer/optimalizer.h'.
 *
 */

#include "optimalizer/optimalizer.h"
#include "tokenizer/tokenizer.h"
#include <unity/unity.h>
#include <stdlib.h>

// ---- GLOBALS ----

fl_token_arr tok_array;
fl_char *source =
    "5 # number of comments in this file (must be the first token) #"
    "some = thing;"
    "# Hi! #"
    "# There is some comment too #"
    "\"But here is a string\""
    "#I guess it's the last one...#"
    "#Maybe some \\# escape at last#";

// ---- SETUP FUNCTIONS ----

void setUp()
{
    fl_tok_tokenize(source, &tok_array, "optimalizer test");
}

void tearDown()
{
    fl_token_arr_free(&tok_array);
}

// ---- TESTS ----

void test_opt_remove_comments_Accuracy(void)
{
    fl_size old_count = tok_array._count;
    fl_size comments_num = atoll(tok_array._data[0].value);
    fl_opt_remove_comments(&tok_array);

    TEST_ASSERT_EQUAL_UINT64(comments_num, old_count - tok_array._count);
}

// ---- ENTRY ----

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_opt_remove_comments_Accuracy);

    return UNITY_END();
}