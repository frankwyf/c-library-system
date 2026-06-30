#include "unity.h"
#include <string.h>

#include "librarian.h"
#include "structures.h"

/* ---- Test fixtures ---- */

static Book g_books[8];
static Library g_lib;

void setUp(void) {
    memset(g_books, 0, sizeof(g_books));

    strncpy(g_books[0].author, "Frank Herbert", MAX_AUTHOR_LEN - 1);
    strncpy(g_books[0].title, "Dune", MAX_TITLE_LEN - 1);
    strncpy(g_books[1].author, "Isaac Asimov", MAX_AUTHOR_LEN - 1);
    strncpy(g_books[1].title, "I, Robot", MAX_TITLE_LEN - 1);

    g_lib.book_list    = g_books;
    g_lib.num_books    = 2;
    g_lib.max_books    = 8;
    g_lib.max_borrowed = 4;
    memset(&g_lib.the_user, 0, sizeof(User));
}

void tearDown(void) {
}

/* ---- Tests: librarian_do_add ---- */

void test_add_book_success(void) {
    int rc = librarian_do_add(&g_lib, "William Gibson", "Neuromancer");
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(3, g_lib.num_books);
    TEST_ASSERT_EQUAL_STRING("William Gibson", g_lib.book_list[2].author);
    TEST_ASSERT_EQUAL_STRING("Neuromancer", g_lib.book_list[2].title);
    TEST_ASSERT_EQUAL_INT(0, g_lib.book_list[2].is_borrowed);
}

void test_add_book_when_full(void) {
    g_lib.num_books = g_lib.max_books; /* pretend catalogue is full */
    int rc          = librarian_do_add(&g_lib, "Author", "Title");
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
    int rc                         = librarian_do_remove(&g_lib, 0);
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
    int rc          = librarian_do_remove(&g_lib, 0);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

/* ---- Tests: string truncation / boundary ---- */

void test_add_long_author_truncated(void) {
    /* Author string longer than MAX_AUTHOR_LEN should be truncated */
    char long_author[128];
    memset(long_author, 'A', sizeof(long_author) - 1);
    long_author[sizeof(long_author) - 1] = '\0';

    int rc = librarian_do_add(&g_lib, long_author, "Short Title");
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_TRUE(strlen(g_lib.book_list[2].author) <
                     (size_t)MAX_AUTHOR_LEN);
    TEST_ASSERT_EQUAL_CHAR('\0', g_lib.book_list[2].author[MAX_AUTHOR_LEN - 1]);
}

void test_add_long_title_truncated(void) {
    char long_title[128];
    memset(long_title, 'T', sizeof(long_title) - 1);
    long_title[sizeof(long_title) - 1] = '\0';

    int rc = librarian_do_add(&g_lib, "Short Author", long_title);
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_TRUE(strlen(g_lib.book_list[2].title) < (size_t)MAX_TITLE_LEN);
}

void test_add_empty_strings(void) {
    int rc = librarian_do_add(&g_lib, "", "");
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_STRING("", g_lib.book_list[2].author);
    TEST_ASSERT_EQUAL_STRING("", g_lib.book_list[2].title);
}

void test_remove_shifts_all_entries(void) {
    /* Add a third book, remove index 0, verify order of remaining two */
    librarian_do_add(&g_lib, "Author C", "Title C");
    TEST_ASSERT_EQUAL_INT(3, g_lib.num_books);

    librarian_do_remove(&g_lib, 0);
    TEST_ASSERT_EQUAL_INT(2, g_lib.num_books);
    TEST_ASSERT_EQUAL_STRING("Isaac Asimov", g_lib.book_list[0].author);
    TEST_ASSERT_EQUAL_STRING("Author C", g_lib.book_list[1].author);
}

void test_add_then_remove_then_add(void) {
    /* Verify catalogue can be reused after removal */
    librarian_do_add(&g_lib, "New Author", "New Title");
    TEST_ASSERT_EQUAL_INT(3, g_lib.num_books);

    librarian_do_remove(&g_lib, 2);
    TEST_ASSERT_EQUAL_INT(2, g_lib.num_books);

    int rc = librarian_do_add(&g_lib, "Another Author", "Another Title");
    TEST_ASSERT_EQUAL_INT(LIB_OK, rc);
    TEST_ASSERT_EQUAL_INT(3, g_lib.num_books);
    TEST_ASSERT_EQUAL_STRING("Another Author", g_lib.book_list[2].author);
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
    RUN_TEST(test_add_long_author_truncated);
    RUN_TEST(test_add_long_title_truncated);
    RUN_TEST(test_add_empty_strings);
    RUN_TEST(test_remove_shifts_all_entries);
    RUN_TEST(test_add_then_remove_then_add);
    return UNITY_END();
}
