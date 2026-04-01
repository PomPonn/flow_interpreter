/**
 * ---------- file.test.c ----------
 *
 * Defines tests for 'src/tools/file.h'.
 *
 */

#include "tools/file.h"
#include <unity/unity.h>
#include <stdio.h>
#include <string.h>

// ---------- GLOBALS ----------

const fl_char buffer[] = "This is some content to be added to the file.\nThat's awesome!";
const fl_char filename[] = "file.test.txt";

// ---------- SETUP FUNCTIONS ----------

void setUp()
{
    // open file in binary mode for compatibility
    FILE *file = fopen(filename, "wb");
    assert(file);
    fwrite(buffer, sizeof(fl_char), strlen(buffer), file);
    fclose(file);
}

void tearDown()
{
    int code = remove(filename);
    assert(code == 0);
}

// ---------- TESTS ----------

void test_file_read_ContentMatch(void)
{
    fl_char *content = fl_file_read(filename);
    TEST_ASSERT_NOT_EQUAL(FL_NULL, content);

    TEST_ASSERT_EQUAL_size_t(strlen(buffer), strlen(content));
    TEST_ASSERT_EQUAL_INT(0, strcmp(buffer, content));

    fl_file_free(&content);
}

void test_file_append_ContentMatch(void)
{
    const fl_char append_me[] = "\nHi There!\n";

    fl_file_append(filename, append_me);

    fl_char *content = fl_file_read(filename);
    TEST_ASSERT_NOT_EQUAL(FL_NULL, content);

    fl_char *appended_content = content + strlen(buffer);

    TEST_ASSERT_EQUAL_size_t(strlen(append_me), strlen(appended_content));
    TEST_ASSERT_EQUAL_INT(0, strcmp(append_me, appended_content));

    fl_file_free(&content);
}

// ---------- ENTRY ----------

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_file_read_ContentMatch);
    RUN_TEST(test_file_append_ContentMatch);

    return UNITY_END();
}