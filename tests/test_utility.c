#include "unity.h"
#include <string.h>

#include "utility.h"

void setUp(void) {
}
void tearDown(void) {
}

/* ---- Tests: util_trim_newline ---- */

void test_trim_removes_lf(void) {
    char s[] = "hello\n";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("hello", s);
}

void test_trim_removes_crlf(void) {
    char s[] = "hello\r\n";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("hello", s);
}

void test_trim_removes_standalone_cr(void) {
    char s[] = "hello\r";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("hello", s);
}

void test_trim_no_newline_unchanged(void) {
    char s[] = "hello";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("hello", s);
}

void test_trim_empty_string_safe(void) {
    char s[] = "";
    util_trim_newline(s); /* must not crash */
    TEST_ASSERT_EQUAL_STRING("", s);
}

void test_trim_only_newline(void) {
    char s[] = "\n";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("", s);
}

void test_trim_only_crlf(void) {
    char s[] = "\r\n";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("", s);
}

void test_trim_multiple_trailing_newlines(void) {
    char s[] = "abc\n\n\n";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("abc", s);
}

void test_trim_does_not_touch_embedded_newline(void) {
    /* Only trailing characters should be removed */
    char s[] = "ab\ncd";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("ab\ncd", s);
}

void test_trim_preserves_spaces(void) {
    char s[] = "  spaced  \n";
    util_trim_newline(s);
    TEST_ASSERT_EQUAL_STRING("  spaced  ", s);
}

/* ---- Runner ---- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_trim_removes_lf);
    RUN_TEST(test_trim_removes_crlf);
    RUN_TEST(test_trim_removes_standalone_cr);
    RUN_TEST(test_trim_no_newline_unchanged);
    RUN_TEST(test_trim_empty_string_safe);
    RUN_TEST(test_trim_only_newline);
    RUN_TEST(test_trim_only_crlf);
    RUN_TEST(test_trim_multiple_trailing_newlines);
    RUN_TEST(test_trim_does_not_touch_embedded_newline);
    RUN_TEST(test_trim_preserves_spaces);
    return UNITY_END();
}
