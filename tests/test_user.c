#include "unity.h"
#include <string.h>

#include "structures.h"
#include "user.h"

/* ---- Test fixtures (file-scope so setUp can reset them safely) ---- */

static Book    g_books[8];
static Library g_lib;

void setUp(void) {
    memset(g_books, 0, sizeof(g_books));

    strncpy(g_books[0].author, "Frank Herbert",  MAX_AUTHOR_LEN - 1);
    strncpy(g_books[0].title,  "Dune",           MAX_TITLE_LEN  - 1);
    strncpy(g_books[1].author, "Isaac Asimov",   MAX_AUTHOR_LEN - 1);
    strncpy(g_books[1].title,  "I, Robot",       MAX_TITLE_LEN  - 1);
    strncpy(g_books[2].author, "William Gibson", MAX_AUTHOR_LEN - 1);
    strncpy(g_books[2].title,  "Neuromancer",    MAX_TITLE_LEN  - 1);

    g_lib.book_list    = g_books;
    g_lib.num_books    = 3;
    g_lib.max_books    = 8;
    g_lib.max_borrowed = 2;
    memset(&g_lib.the_user, 0, sizeof(User));
}

void tearDown(void) {}

/* ---- Tests: user_do_borrow ---- */

void test_borrow_success(void) {
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list,
                            g_lib.num_books, g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(1, g_lib.the_user.num_borrowed);
    TEST_ASSERT_EQUAL_INT(1, g_lib.book_list[0].is_borrowed);
    TEST_ASSERT_EQUAL_PTR(&g_lib.book_list[0], g_lib.the_user.borrowed[0]);
}

void test_borrow_when_at_limit(void) {
    /* Fill borrow list to capacity (max_borrowed = 2) */
    user_do_borrow(&g_lib.the_user, g_lib.book_list,
                   g_lib.num_books, g_lib.max_borrowed, 0);
    user_do_borrow(&g_lib.the_user, g_lib.book_list,
                   g_lib.num_books, g_lib.max_borrowed, 1);
    /* Third attempt must fail */
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list,
                            g_lib.num_books, g_lib.max_borrowed, 2);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
    TEST_ASSERT_EQUAL_INT(2, g_lib.the_user.num_borrowed);
}

void test_borrow_already_borrowed_book(void) {
    g_lib.book_list[0].is_borrowed = 1;
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list,
                            g_lib.num_books, g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
    TEST_ASSERT_EQUAL_INT(0, g_lib.the_user.num_borrowed);
}

void test_borrow_invalid_positive_index(void) {
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list,
                            g_lib.num_books, g_lib.max_borrowed, 99);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

void test_borrow_negative_index(void) {
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list,
                            g_lib.num_books, g_lib.max_borrowed, -1);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

/* ---- Tests: user_do_return ---- */

void test_return_success(void) {
    user_do_borrow(&g_lib.the_user, g_lib.book_list,
                   g_lib.num_books, g_lib.max_borrowed, 0);
    int rc = user_do_return(&g_lib.the_user, 0);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(0, g_lib.the_user.num_borrowed);
    TEST_ASSERT_EQUAL_INT(0, g_lib.book_list[0].is_borrowed);
    TEST_ASSERT_NULL(g_lib.the_user.borrowed[0]);
}

void test_return_when_nothing_borrowed(void) {
    int rc = user_do_return(&g_lib.the_user, 0);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

void test_return_invalid_index(void) {
    user_do_borrow(&g_lib.the_user, g_lib.book_list,
                   g_lib.num_books, g_lib.max_borrowed, 0);
    int rc = user_do_return(&g_lib.the_user, 5);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
    TEST_ASSERT_EQUAL_INT(1, g_lib.the_user.num_borrowed);
}

void test_return_compacts_borrow_list(void) {
    user_do_borrow(&g_lib.the_user, g_lib.book_list,
                   g_lib.num_books, g_lib.max_borrowed, 0);
    user_do_borrow(&g_lib.the_user, g_lib.book_list,
                   g_lib.num_books, g_lib.max_borrowed, 1);
    /* Return the first entry; second should shift to index 0 */
    user_do_return(&g_lib.the_user, 0);
    TEST_ASSERT_EQUAL_INT(1, g_lib.the_user.num_borrowed);
    TEST_ASSERT_EQUAL_STRING("Isaac Asimov", g_lib.the_user.borrowed[0]->author);
}

void test_return_book_becomes_available_again(void) {
    user_do_borrow(&g_lib.the_user, g_lib.book_list,
                   g_lib.num_books, g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(1, g_lib.book_list[0].is_borrowed);
    user_do_return(&g_lib.the_user, 0);
    TEST_ASSERT_EQUAL_INT(0, g_lib.book_list[0].is_borrowed);
}

/* ---- Runner ---- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_borrow_success);
    RUN_TEST(test_borrow_when_at_limit);
    RUN_TEST(test_borrow_already_borrowed_book);
    RUN_TEST(test_borrow_invalid_positive_index);
    RUN_TEST(test_borrow_negative_index);
    RUN_TEST(test_return_success);
    RUN_TEST(test_return_when_nothing_borrowed);
    RUN_TEST(test_return_invalid_index);
    RUN_TEST(test_return_compacts_borrow_list);
    RUN_TEST(test_return_book_becomes_available_again);
    return UNITY_END();
}
