#include "unity.h"
#include <string.h>

#include "structures.h"
#include "user.h"

/* ---- Test fixtures (file-scope so setUp can reset them safely) ---- */

static Book g_books[8];
static Library g_lib;

void setUp(void) {
    memset(g_books, 0, sizeof(g_books));

    strncpy(g_books[0].author, "Frank Herbert", MAX_AUTHOR_LEN - 1);
    strncpy(g_books[0].title, "Dune", MAX_TITLE_LEN - 1);
    strncpy(g_books[1].author, "Isaac Asimov", MAX_AUTHOR_LEN - 1);
    strncpy(g_books[1].title, "I, Robot", MAX_TITLE_LEN - 1);
    strncpy(g_books[2].author, "William Gibson", MAX_AUTHOR_LEN - 1);
    strncpy(g_books[2].title, "Neuromancer", MAX_TITLE_LEN - 1);

    g_lib.book_list    = g_books;
    g_lib.num_books    = 3;
    g_lib.max_books    = 8;
    g_lib.max_borrowed = 2;
    memset(&g_lib.the_user, 0, sizeof(User));
}

void tearDown(void) {
}

/* ---- Tests: user_do_borrow ---- */

void test_borrow_success(void) {
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                            g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(1, g_lib.the_user.num_borrowed);
    TEST_ASSERT_EQUAL_INT(1, g_lib.book_list[0].is_borrowed);
    TEST_ASSERT_EQUAL_PTR(&g_lib.book_list[0], g_lib.the_user.borrowed[0]);
}

void test_borrow_when_at_limit(void) {
    /* Fill borrow list to capacity (max_borrowed = 2) */
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 0);
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 1);
    /* Third attempt must fail */
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                            g_lib.max_borrowed, 2);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
    TEST_ASSERT_EQUAL_INT(2, g_lib.the_user.num_borrowed);
}

void test_borrow_already_borrowed_book(void) {
    g_lib.book_list[0].is_borrowed = 1;
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                            g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
    TEST_ASSERT_EQUAL_INT(0, g_lib.the_user.num_borrowed);
}

void test_borrow_invalid_positive_index(void) {
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                            g_lib.max_borrowed, 99);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

void test_borrow_negative_index(void) {
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                            g_lib.max_borrowed, -1);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

/* ---- Tests: user_do_return ---- */

void test_return_success(void) {
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 0);
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
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 0);
    int rc = user_do_return(&g_lib.the_user, 5);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
    TEST_ASSERT_EQUAL_INT(1, g_lib.the_user.num_borrowed);
}

void test_return_compacts_borrow_list(void) {
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 0);
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 1);
    /* Return the first entry; second should shift to index 0 */
    user_do_return(&g_lib.the_user, 0);
    TEST_ASSERT_EQUAL_INT(1, g_lib.the_user.num_borrowed);
    TEST_ASSERT_EQUAL_STRING("Isaac Asimov",
                             g_lib.the_user.borrowed[0]->author);
}

void test_return_book_becomes_available_again(void) {
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(1, g_lib.book_list[0].is_borrowed);
    user_do_return(&g_lib.the_user, 0);
    TEST_ASSERT_EQUAL_INT(0, g_lib.book_list[0].is_borrowed);
}

/* ---- Tests: borrow-return cycles ---- */

void test_borrow_return_borrow_same_book(void) {
    /* User should be able to re-borrow a book after returning it */
    int rc;
    rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                        g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);

    rc = user_do_return(&g_lib.the_user, 0);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);

    rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                        g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(1, g_lib.the_user.num_borrowed);
}

void test_borrow_all_then_return_all(void) {
    /* Fill the borrow list to capacity, then return everything */
    int i, rc;
    for (i = 0; i < g_lib.max_borrowed; i++) {
        rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                            g_lib.max_borrowed, i);
        TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    }
    TEST_ASSERT_EQUAL_INT(g_lib.max_borrowed, g_lib.the_user.num_borrowed);

    /* Return from the back to avoid shifting complications */
    for (i = g_lib.max_borrowed - 1; i >= 0; i--) {
        rc = user_do_return(&g_lib.the_user, i);
        TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    }
    TEST_ASSERT_EQUAL_INT(0, g_lib.the_user.num_borrowed);
}

void test_return_middle_compacts_correctly(void) {
    /* Borrow 0, 1, 2 (requires max_borrowed >= 3 → set to 3 for this test) */
    g_lib.max_borrowed = 3;
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 0);
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 1);
    user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                   g_lib.max_borrowed, 2);

    /* Return the middle one (borrow index 1) */
    user_do_return(&g_lib.the_user, 1);
    TEST_ASSERT_EQUAL_INT(2, g_lib.the_user.num_borrowed);
    TEST_ASSERT_EQUAL_STRING("Frank Herbert",
                             g_lib.the_user.borrowed[0]->author);
    TEST_ASSERT_EQUAL_STRING("William Gibson",
                             g_lib.the_user.borrowed[1]->author);
    TEST_ASSERT_NULL(g_lib.the_user.borrowed[2]);
}

void test_borrow_zero_index_boundary(void) {
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                            g_lib.max_borrowed, 0);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_STRING("Dune", g_lib.the_user.borrowed[0]->title);
}

void test_borrow_last_index_boundary(void) {
    int rc = user_do_borrow(&g_lib.the_user, g_lib.book_list, g_lib.num_books,
                            g_lib.max_borrowed, g_lib.num_books - 1);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_STRING("Neuromancer", g_lib.the_user.borrowed[0]->title);
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
    RUN_TEST(test_borrow_return_borrow_same_book);
    RUN_TEST(test_borrow_all_then_return_all);
    RUN_TEST(test_return_middle_compacts_correctly);
    RUN_TEST(test_borrow_zero_index_boundary);
    RUN_TEST(test_borrow_last_index_boundary);
    return UNITY_END();
}
