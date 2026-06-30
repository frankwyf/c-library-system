#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "librarian.h"
#include "library.h"
#include "structures.h"
#include "user.h"
#include "utility.h"

/* ------------------------------------------------------------------ */

int library_init(const char *book_file, Library *lib) {
    FILE *fp;
    int count;

    lib->max_books    = MAX_BOOKS;
    lib->max_borrowed = MAX_BORROWED;
    lib->num_books    = 0;

    lib->book_list = (Book *)calloc((size_t)lib->max_books, sizeof(Book));
    if (!lib->book_list) {
        fprintf(stderr, "Error: out of memory\n");
        return LIB_ERR;
    }

    fp = fopen(book_file, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open book file '%s'\n", book_file);
        free(lib->book_list);
        lib->book_list = NULL;
        return LIB_ERR;
    }

    count = library_read_books(fp, lib->book_list, lib->max_books);
    fclose(fp);

    if (count < 0) {
        free(lib->book_list);
        lib->book_list = NULL;
        return LIB_ERR;
    }

    lib->num_books = count;
    memset(&lib->the_user, 0, sizeof(User));
    return LIB_OK;
}

int library_read_books(FILE *fp, Book *book_list, int max_books) {
    char line[128];
    int count = 0;
    int row   = 0; /* 0 = expecting author line, 1 = expecting title line */

    while (fgets(line, (int)sizeof(line), fp) != NULL) {
        util_trim_newline(line);

        if (line[0] == '\0') /* skip blank separator lines */
            continue;

        if (count >= max_books) {
            fprintf(
                stderr,
                "Warning: catalogue full (%d entries), extra books ignored\n",
                max_books);
            break;
        }

        if (row == 0) {
            strncpy(book_list[count].author, line, MAX_AUTHOR_LEN - 1);
            book_list[count].author[MAX_AUTHOR_LEN - 1] = '\0';
            row                                         = 1;
        } else {
            strncpy(book_list[count].title, line, MAX_TITLE_LEN - 1);
            book_list[count].title[MAX_TITLE_LEN - 1] = '\0';
            book_list[count].is_borrowed              = 0;
            count++;
            row = 0;
        }
    }

    return count;
}

void library_free(Library *lib) {
    if (!lib)
        return;
    memset(&lib->the_user, 0,
           sizeof(User)); /* clear dangling borrowed[] ptrs */
    free(lib->book_list);
    lib->book_list = NULL;
    lib->num_books = 0;
}

void library_cli(const char *book_file) {
    Library lib;
    int running = 1;

    printf("\n========================================\n");
    printf("   C Library Management System v1.0\n");
    printf("========================================\n\n");

    if (library_init(book_file, &lib) != LIB_OK) {
        fprintf(stderr, "Startup failed. Exiting.\n");
        return;
    }
    printf("Loaded %d book(s) from \"%s\".\n", lib.num_books, book_file);

    while (running) {
        printf("\n--- Main Menu ---\n");
        printf("  1  User login\n");
        printf("  2  Librarian login\n");
        printf("  3  Exit\n");
        printf("Choice: ");
        int choice = util_read_option();

        switch (choice) {
        case 1:
            printf("\n[User session]\n");
            user_cli(&lib);
            break;
        case 2:
            printf("\n[Librarian session]\n");
            librarian_cli(&lib);
            break;
        case 3:
            running = 0;
            printf("\nGoodbye!\n\n");
            break;
        default:
            printf("Invalid option. Enter 1, 2, or 3.\n");
            break;
        }
    }

    library_free(&lib);
}
