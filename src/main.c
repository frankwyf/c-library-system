#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "1.0.0"
#endif

/*
 * C Library Management System
 *
 * Usage:
 *   library [--help | --version] <book-file>
 *
 * Example:
 *   ./build/library data/books.txt
 */

static void print_help(const char *prog) {
    printf("Usage: %s [OPTION] <book-file>\n", prog);
    printf("\n");
    printf("A console-based library management system.\n");
    printf("\n");
    printf("Options:\n");
    printf("  --help       Show this help message and exit\n");
    printf("  --version    Show version information and exit\n");
    printf("\n");
    printf("Example:\n");
    printf("  %s data/books.txt\n", prog);
}

static void print_version(void) {
    printf("c-library-system %s\n", PROJECT_VERSION);
    printf("License: MIT\n");
    printf("https://github.com/frankwyf/c-library-system\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }
    if (argc == 2 && strcmp(argv[1], "--version") == 0) {
        print_version();
        return EXIT_SUCCESS;
    }
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <book-file>\n", argv[0]);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return EXIT_FAILURE;
    }
    library_cli(argv[1]);
    return EXIT_SUCCESS;
}
