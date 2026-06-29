#include "unity.h"
#include <string.h>

#include "structures.h"
#include "librarian.h"

/* ---- Test fixtures ---- */

static Book    g_books[8];
static Library g_lib;

void setUp(void) {
    memset(g_books, 0, sizeof(g_books));

    strncpy(g_books[0].author, "Frank Herbert", MAX_AUTHOR_LEN - 1);
    strncpy(g_books[0].title,  "Dune",          MAX_TITLE_LEN  - 1);
    strncpy(g_books[1].author, "Isaac Asimov",  MAX_AUTHOR_LEN - 1);
    strncpy(g_books[1].title,  "I, Robot",      MAX_TITLE_LEN  - 1);

    g_lib.book_list    = g_books;
    g_lib.num_books    = 2;
    g_lib.max_books    = 8;
    g_lib.max_borrowed = 4;
    memset(&g_lib.the_user, 0, sizeof(User));
}

void tearDown(void) {}

/* ---- Tests: librarian_do_add ---- */

void test_add_book_success(void) {
    int rc = librarian_do_add(&g_lib, "William Gibson", "Neuromancer");
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(3, g_lib.num_books);
    TEST_ASSERT_EQUAL_STRING("William Gibson", g_lib.book_list[2].author);
    TEST_ASSERT_EQUAL_STRING("Neuromancer",    g_lib.book_list[2].title);
    TEST_ASSERT_EQUAL_INT(0, g_lib.book_list[2].is_borrowed);
}

void test_add_book_when_full(void) {
    g_lib.num_books = g_lib.max_books; /* pretend catalogue is full */
    int rc = librarian_do_add(&g_lib, "Author", "Title");
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

void test_add_increments_count(void) {
    int before = g_lib.num_books;
    librarian_do_add(&g_lib, "Ray Bradbury", "Fahrenheit 451");
    TEST_ASSERT_EQUAL_INT(before + 1, g_lib.num_books);
}

/* ---- Tests: librarian_do_remove ---- */

void test_remove_success(void) {
    int rc = librarian_do_remove(&g_lib, 0);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(1, g_lib.num_books);
    /* Entry 1 shifts to position 0 */
    TEST_ASSERT_EQUAL_STRING("Isaac Asimov", g_lib.book_list[0].author);
}

void test_remove_last_entry(void) {
    int rc = librarian_do_remove(&g_lib, 1);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(1, g_lib.num_books);
    TEST_ASSERT_EQUAL_STRING("Frank Herbert", g_lib.book_list[0].author);
}

void test_remove_borrowed_book_fails(void) {
    g_lib.book_list[0].is_borrowed = 1;
    int rc = librarian_do_remove(&g_lib, 0);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
    TEST_ASSERT_EQUAL_INT(2, g_lib.num_books); /* unchanged */
}

void test_remove_invalid_positive_index(void) {
    int rc = librarian_do_remove(&g_lib, 99);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
    TEST_ASSERT_EQUAL_INT(2, g_lib.num_books);
}

void test_remove_negative_index(void) {
    int rc = librarian_do_remove(&g_lib, -1);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

void test_remove_from_empty_catalogue(void) {
    g_lib.num_books = 0;
    int rc = librarian_do_remove(&g_lib, 0);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

/* ---- Runner ---- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_add_book_success);
    RUN_TEST(test_add_book_when_full);
    RUN_TEST(test_add_increments_count);
    RUN_TEST(test_remove_success);
    RUN_TEST(test_remove_last_entry);
    RUN_TEST(test_remove_borrowed_book_fails);
    RUN_TEST(test_remove_invalid_positive_index);
    RUN_TEST(test_remove_negative_index);
    RUN_TEST(test_remove_from_empty_catalogue);
    return UNITY_END();
}
