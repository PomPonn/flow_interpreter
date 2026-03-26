/**
 * ---- ast_node.test.c ----
 *
 * Defines tests for 'src/ast/ast_node.h'.
 *
 */

#include "ast/ast_node.h"
#include <unity/unity.h>

// ---- GLOBALS ----

fl_ast_node *parent;
fl_ast_node *node;
fl_token_location loc_info = {
    .column = 0,
    .line = 0,
    .description = "",
};

// ---- SETUP FUNCTIONS ----

void setUp()
{
    fl_ast_node_init(&parent, FL_AST_NODE_TYPE_UNKNOWN, FL_NULL, FL_NULL, loc_info);
    fl_ast_node_init(&node, FL_AST_NODE_TYPE_UNKNOWN, FL_NULL, parent, loc_info);
}

void tearDown()
{
    fl_ast_node_free(&parent);
}

// ---- TESTS ----

void test_ast_node_init_ParentHandling(void)
{
    TEST_ASSERT_EQUAL_PTR(parent, node->parent);
    TEST_ASSERT_EQUAL_PTR(node, fl_ast_node_peek_child(parent));
}

void test_ast_node_peek_child_ReturnValue(void)
{
    TEST_ASSERT_EQUAL_PTR(node, fl_ast_node_peek_child(parent));
    TEST_ASSERT_EQUAL_PTR(FL_NULL, fl_ast_node_peek_child(node));
}

void test_ast_node_add_child_CorrectRelation(void)
{
    fl_ast_node *new_node;
    fl_ast_node_init(&new_node, FL_AST_NODE_TYPE_UNKNOWN, FL_NULL, FL_NULL, loc_info);
    fl_ast_node_add_child(node, new_node);

    TEST_ASSERT_EQUAL_PTR(node, new_node->parent);
    TEST_ASSERT_EQUAL_PTR(new_node, node->child);

    fl_ast_node *new_node2;
    fl_ast_node_init(&new_node2, FL_AST_NODE_TYPE_UNKNOWN, FL_NULL, FL_NULL, loc_info);
    fl_ast_node_add_child(node, new_node2);

    TEST_ASSERT_EQUAL_PTR(node, new_node2->parent);
    TEST_ASSERT_EQUAL_PTR(new_node2, fl_ast_node_peek_child(node));
}

// ---- ENTRY ----

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_ast_node_peek_child_ReturnValue);
    RUN_TEST(test_ast_node_init_ParentHandling);
    RUN_TEST(test_ast_node_add_child_CorrectRelation);

    return UNITY_END();
}