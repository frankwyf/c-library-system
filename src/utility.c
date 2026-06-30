#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

int util_read_option(void) {
    char line[80];
    if (fgets(line, sizeof(line), stdin) == NULL)
        return -1;
    return (int)strtol(line, NULL, 10);
}

void util_trim_newline(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
        s[--len] = '\0';
}

int util_read_index(const char *prompt) {
    char line[80];
    char *end;
    long val;
    int i;

    printf("%s", prompt);
    fflush(stdout);

    if (fgets(line, sizeof(line), stdin) == NULL)
        return -1;

    /* Reject anything that contains non-digit characters before newline */
    for (i = 0; line[i] != '\0' && line[i] != '\n' && line[i] != '\r'; i++) {
        if (!isdigit((unsigned char)line[i]))
            return -1;
    }
    if (i == 0)
        return -1;

    val = strtol(line, &end, 10);
    if (end == line)
        return -1;

    return (int)val;
}
