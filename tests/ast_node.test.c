/**
 * ---- ast_node.test.c ----
 *
 * Defines tests for 'src/ast/ast_node.h'.
 *
 */

#include "ast/ast_node.h"
#include <unity/unity.h>

fl_ast_node *helper;
fl_ast_node *node;
fl_file_location_info loc_info = {
    .column = 0,
    .line = 0,
    .description = "",
};

void setUp()
{
    fl_ast_node_init(&helper, FL_AST_NODE_TYPE_BLOCK, FL_NULL, FL_NULL, loc_info);
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_BLOCK, FL_NULL, helper, loc_info);
}
void tearDown()
{
    fl_ast_node_free(&helper);
    fl_ast_node_free(&node);
}

void test_ast_node_init_ParentHandling(void)
{
    TEST_ASSERT_EQUAL_PTR(helper, node->parent);
    TEST_ASSERT_EQUAL_PTR(node, fl_ast_node_peek_child(helper));
}

void test_ast_node_peek_child_ReturnValue(void)
{
    TEST_ASSERT_EQUAL_PTR(node, fl_ast_node_peek_child(helper));
    TEST_ASSERT_EQUAL_PTR(FL_NULL, fl_ast_node_peek_child(node));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_ast_node_init_ParentHandling);
    RUN_TEST(test_ast_node_peek_child_ReturnValue);
    return UNITY_END();
}