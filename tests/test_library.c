#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"

/* ---- setUp / tearDown (called by Unity before/after each test) ---- */

void setUp(void)    {}
void tearDown(void) {}

/* ---- Helper: create an anonymous temp file with given content ---- */

static FILE *make_temp_file(const char *content) {
    FILE *fp = tmpfile();
    if (!fp) return NULL;
    fputs(content, fp);
    rewind(fp);
    return fp;
}

/* ---- Tests: library_read_books ---- */

void test_read_two_books(void) {
    Book  books[8];
    FILE *fp;
    int   n;

    memset(books, 0, sizeof(books));
    fp = make_temp_file("Frank Herbert\nDune\n\nIsaac Asimov\nI, Robot\n");
    TEST_ASSERT_NOT_NULL(fp);

    n = library_read_books(fp, books, 8);
    fclose(fp);

    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_STRING("Frank Herbert", books[0].author);
    TEST_ASSERT_EQUAL_STRING("Dune",          books[0].title);
    TEST_ASSERT_EQUAL_INT(0, books[0].is_borrowed);
    TEST_ASSERT_EQUAL_STRING("Isaac Asimov",  books[1].author);
    TEST_ASSERT_EQUAL_STRING("I, Robot",      books[1].title);
}

void test_read_empty_file(void) {
    Book  books[8];
    FILE *fp;
    int   n;

    fp = make_temp_file("");
    TEST_ASSERT_NOT_NULL(fp);

    n = library_read_books(fp, books, 8);
    fclose(fp);

    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_read_respects_max_books(void) {
    Book  books[2];
    FILE *fp;
    int   n;

    memset(books, 0, sizeof(books));
    fp = make_temp_file("Frank Herbert\nDune\n\nIsaac Asimov\nI, Robot\n");
    TEST_ASSERT_NOT_NULL(fp);

    n = library_read_books(fp, books, 1); /* cap at 1 */
    fclose(fp);

    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_STRING("Frank Herbert", books[0].author);
}

void test_read_strips_trailing_newlines(void) {
    Book  books[2];
    FILE *fp;
    int   n;
    size_t len;

    memset(books, 0, sizeof(books));
    fp = make_temp_file("Frank Herbert\nDune\n\n");
    TEST_ASSERT_NOT_NULL(fp);

    n = library_read_books(fp, books, 2);
    fclose(fp);

    TEST_ASSERT_EQUAL_INT(1, n);
    len = strlen(books[0].author);
    TEST_ASSERT_TRUE(len > 0 && books[0].author[len - 1] != '\n');
    len = strlen(books[0].title);
    TEST_ASSERT_TRUE(len > 0 && books[0].title[len - 1] != '\n');
}

/* ---- Tests: library_init ---- */

void test_init_missing_file_returns_err(void) {
    Library lib;
    int rc = library_init("__no_such_file_xyz__.txt", &lib);
    TEST_ASSERT_EQUAL_INT(LIB_ERR, rc);
}

/* ---- Tests: library_free ---- */

void test_free_nulls_book_list(void) {
    Library lib;
    lib.book_list    = (Book *)calloc(4, sizeof(Book));
    lib.num_books    = 1;
    lib.max_books    = 4;
    lib.max_borrowed = 4;
    memset(&lib.the_user, 0, sizeof(User));

    library_free(&lib);

    TEST_ASSERT_NULL(lib.book_list);
    TEST_ASSERT_EQUAL_INT(0, lib.num_books);
}

void test_free_null_pointer_is_safe(void) {
    library_free(NULL); /* must not crash */
    TEST_PASS();
}

/* ---- Runner ---- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_read_two_books);
    RUN_TEST(test_read_empty_file);
    RUN_TEST(test_read_respects_max_books);
    RUN_TEST(test_read_strips_trailing_newlines);
    RUN_TEST(test_init_missing_file_returns_err);
    RUN_TEST(test_free_nulls_book_list);
    RUN_TEST(test_free_null_pointer_is_safe);
    return UNITY_END();
}
