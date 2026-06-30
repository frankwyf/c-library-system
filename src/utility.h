#ifndef LIB_UTILITY_H
#define LIB_UTILITY_H

/*
 * Read an integer menu choice from stdin.
 * Returns the integer value, or -1 if input cannot be parsed.
 */
int util_read_option(void);

/*
 * Remove trailing newline / carriage-return from a string in-place.
 */
void util_trim_newline(char *s);

/*
 * Print prompt, then read a non-negative integer from stdin.
 * Rejects input that contains non-digit characters.
 * Returns the integer, or -1 on invalid input or EOF.
 */
int util_read_index(const char *prompt);

#endif /* LIB_UTILITY_H */
