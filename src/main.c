#include <stdio.h>
#include <stdlib.h>

#include "library.h"

/*
 * C Library Management System
 *
 * Usage:
 *   library <book-file>
 *
 * Example:
 *   ./build/library data/books.txt
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <book-file>\n", argv[0]);
        fprintf(stderr, "Example: %s data/books.txt\n", argv[0]);
        return EXIT_FAILURE;
    }
    library_cli(argv[1]);
    return EXIT_SUCCESS;
}
